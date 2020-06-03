//
//  Character.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/26/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "Character.hpp"

#include "NeutralState.hpp"
#include "PhysicsEngine.hpp"
#include "AirborneNeutralState.hpp"
#include "LandingLagState.hpp"
#include "StageEntity.hpp"
#include "PlatformEntity.hpp"
#include "CharacterState.hpp"
#include "DashState.hpp"
#include "TurnaroundState.hpp"
#include "JumpsquatState.hpp"
#include "AirdodgeState.hpp"
#include "MoveLoader.hpp"

#include "MslInterpreter.hpp"
#include "MslScanner.hpp"

using std::list;
using std::cerr;
using std::cout;
using std::endl;

Character::Character(int id, sf::Vector2f vec) : Entity(id, vec) {
    input_ = nullptr;
    mslIntp = new MslInterpreter(this);
    SetActionState(new AirborneNeutralState(this));
}

Character::~Character() {
    // TODO: Deal with rollback_ and other dynamically allocated data
}

void Character::ProcessInput(const PlayerInput &input) {
    input_ = const_cast<PlayerInput* >(&input);
    actionState_->ProcessInput(input);
}

void Character::Tick() {
    fill = false;
    // Teleport back above stage
    if (Position().y > 1500) {
        SetPosition({Position().x, -1000});
    }
    
    if (data.ftCount_ > 0)  {
        if (--data.ftCount_ == 0) data.fallthrough_ = -1;
    }

    // Do state dependent actions
    switch (actionState_->GetState()) {
        case AIRBORNE:
            break;
        case GROUNDED:
            if (!engine->CheckBoundingBoxCollision(this, data.groundedData.stage)) {
                actionState_->SwitchState(AIRBORNE);
                initAirborneData();
            }
            break;
    }
    
    actionState_->Tick();
}

void Character::RollbackTick() {
    Entity::RollbackTick();
    
    data.actionStateT_ = actionState_->GetStateType();
    data.actionStateData_ = actionState_->data;
    
    rollback_ = data;
}

void Character::Rollback() {
    Entity::Rollback();
    
    delete actionState_;
    
    data = rollback_;
    switch (data.actionStateT_) {
        case LandingLag:
            actionState_ = new LandingLagState(this, true);
            break;
        case Neutral:
            actionState_ = new NeutralState(this);
            break;
        case CharStateType::Dash:
            actionState_ = new DashState(this, true);
            break;
        case Jumpsquat:
            actionState_ = new JumpsquatState(this);
            break;
        case CharStateType::Turnaround:
            actionState_ = new TurnaroundState(this, true);
            break;
        case AirborneNeutral:
            actionState_ = new AirborneNeutralState(this);
            break;
        case CharStateType::Airdodge:
            actionState_ = new AirdodgeState(this, true);
            break;
    }
    actionState_->setData(data.actionStateData_);
}

void Character::HandleCollision(const Entity &entity, sf::Vector2f pv) {
    if (entity.id == data.fallthrough_) {
        return;
    }
    
    if (entity.Type() == CHARACTER) {
        fill = true;
    }
    
    if (actionState_->GetState() == AIRBORNE) {
        if (entity.Type() == STAGE) {
            // Apply the push vector to prevent overlap
            Transform(pv);
            
            if (pv.x == 0 && pv.y < 0 && data.velocity_.y > 0) {
                // Land on the stage
                NullVelocityY();
                actionState_->SwitchState(GROUNDED);
                data.groundedData.stage = dynamic_cast<const StageEntity*>(&entity);
                return;
            }
        } else if (entity.Type() == PLATFORM && !input_->stick.inDirection(DOWN)) {
            if (pv.x == 0 && pv.y < 0) {
                // The character collided with the platform. Check if the character
                // is above the platform and falling down
                Rectangle b = BoundingBox();
                Rectangle s = entity.BoundingBox();
                if (data.velocity_.y > 0 && (b.y + b.h) - data.velocity_.y < s.y) {
                    // Land on the platform
                    NullVelocityY();
                    // Apply the push vector to prevent overlap
                    Transform(pv);
                    
                    actionState_->SwitchState(GROUNDED);
                    data.groundedData.stage = dynamic_cast<const StageEntity*>(&entity);
                    return;
                }
            }
        }
    }
    
    actionState_->HandleCollision(entity, pv);
}

void Character::Jump(JumpType type, bool fullhop) {
    if (actionState_->GetState() != AIRBORNE) {
        std::cerr << "ERROR: JUMP INVALID STATE " << std::endl;
        return;
    }
    
    if (data.airborneData.fastfall) {
        data.airborneData.fastfall = false;
    }
    
    float xv;
    float yv = fullhop ? attr.FullhopJump : attr.ShorthopJump;
    if (type == JLEFT || type == JUP || type == JRIGHT) {
        initAirborneData();
    }
    switch (type) {
        case JLEFT:
            xv = - attr.MaxGroundSpeed;
            break;
        case JUP:
            xv = 0.f;
            break;
        case JRIGHT:
            xv = attr.MaxGroundSpeed;
            break;
        case DJUMP:
            if (data.airborneData.jumps > 0) {
                data.airborneData.jumps--;
                xv = 0.f;
                yv = attr.DoubleJump;
            } else {
                // No jumps left
                return;
            }
            break;
    }
    SetActionState(new AirborneNeutralState(this));
    data.velocity_.y = yv;
    data.velocity_.x = xv;
}

void Character::Dash(float m) {
    if (actionState_->GetState() != GROUNDED) {
        std::cerr << "ERROR: DASH INVALID STATE " << std::endl;
        return;
    }
    
    if (abs(m) > 1.f) {
        std::cerr << "INVALID DASH MODIFIER" << std::endl;
        return;
    }
    
    SetDirection(m < 0 ? -1 : 1);
        
    data.velocity_.x = m * attr.MaxGroundSpeed;
}

void Character::Vector(float v) {
    if (actionState_->GetState() != AIRBORNE) {
        std::cerr << "ERROR: VECTOR INVALID STATE " << std::endl;
        return;
    }
    
    float hyp = input_->stick.hyp();
    if (hyp <= PlayerInput::DEAD_ZONE) {
        ApplyFriction();
        return;
    }
    
    float m = (abs(input_->stick.xAxis) > 60.f ? 60.f : abs(input_->stick.xAxis)) / 60.f;
    if (input_->stick.xAxis < 0) m *= -1;
    
    data.velocity_.x += m * v * attr.AirAccel;
    int sign = data.velocity_.x < 0 ? -1 : 1;
    data.velocity_.x = sign * fmin(attr.MaxAirSpeed, abs(data.velocity_.x));
    
    if (input_->stick.inDirection(DOWN_T) && hyp > 50.f && data.velocity_.y > 0.f) {
        FastFall();
    }
}

void Character::ApplyFriction() {
    data.velocity_.x *= (actionState_->GetState() == GROUNDED) ?
            attr.GroundFriction : attr.AirFriction;
}

void Character::SetActionState(CharacterState *s) {
    data.actionStateT_ = s->GetStateType();
    actionState_ = s;
}

void Character::FallthroughPlatform() {
    if (actionState_->GetState() == AIRBORNE ||
        data.groundedData.stage->Type() != EntityType::PLATFORM) {
        std::cerr << "ERROR Attempt to fall through " << data.groundedData.stage->Type();
        std::cerr << ", WITH STATE " << actionState_->GetState() << std::endl;
        return;
    }
    
    data.fallthrough_ = data.groundedData.stage->id;
    data.ftCount_ = 4;
    initAirborneData();
}

void Character::WallJump(int dir) {
    if (actionState_->GetState() != AIRBORNE) {
        std::cerr << "ERROR Attempt to walljump, state: " << actionState_->GetState() << std::endl;
        return;
    }
    
    if (data.airborneData.walljump) {
        data.velocity_.x = dir * attr.MaxAirSpeed / 1.5f;
        data.velocity_.y = attr.WallJump;
        data.airborneData.walljump = false;
    }
}

void Character::Airdodge() {
    if (actionState_->GetState() != AIRBORNE
        || !data.airborneData.airdodge) {
        std::cerr << "ERROR Attempt to airdodge, state: " << actionState_->GetState();
        std::cerr << ", airdodge: " << data.airborneData.airdodge << std::endl;
        return;
    }
    
    float a = input_->stick.angle();
    data.velocity_.x = cos(a) * attr.AirdodgeVelocity;
    data.velocity_.y = - sin(a) * attr.AirdodgeVelocity;
    data.airborneData.airdodge = false;
}
