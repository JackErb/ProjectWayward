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
#include "MathHelper.hpp"

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
    
    if (entity.Type() == STAGE) {
        // Apply the push vector to prevent overlap
        Transform(pv);
    }
    
    if (actionState_->GetState() == AIRBORNE) {
        if (entity.Type() == STAGE) {            
            if (pv.x == 0 && pv.y < 0 && data.velocity_.y > 0) {
                // Land on the stage
                NullVelocityY();
                actionState_->SwitchState(GROUNDED);
                data.groundedData.stage = dynamic_cast<const StageEntity*>(&entity);
                return;
            }
        } else if (entity.Type() == PLATFORM && !input_->stick.inDirection(DOWN)) {
            if (pv.y <= 0) {
                // The character collided with the platform. Check if the character
                // is above the platform and falling down
                Rectangle b = BoundingBox();
                Rectangle s = entity.BoundingBox();
                
                bool vert_check = (b.y + b.h) - data.velocity_.y * 1.1f < s.y;
                bool horiz_check = (b.x + b.w * 5.f / 8.f) > s.x && (b.x + b.w * 3.f / 8.f) < (s.x + s.w);
                if (data.velocity_.y > 0 && vert_check && horiz_check) {
                    // Land on the platform
                    NullVelocityY();
                    // Apply the push vector to prevent overlap
                    Transform(pv);
                    
                    actionState_->SwitchState(GROUNDED);
                    data.groundedData.stage = dynamic_cast<const StageEntity*>(&entity);
                    return;
                } else {
                    cout << "HERE" << endl;
                }
            } else {
                cout << "HERE2" << endl;
            }
        }
    }
    
    actionState_->HandleCollision(entity, pv);
}

void Character::HandleHit(const Entity *e, const HitboxData &hd) {
    SetActionState(new AirborneNeutralState(this));
    float kb = hd.basekb + hd.kbscale * (data.percent_ / 100.f);
    data.percent_ += hd.dmg;
    
    float angle = hd.angle;
    if (hd.reverse) {
        sf::Vector2f center = geometric_center(hd.hitbox);
        center.x *= e->Direction();
        center += e->Position();
        if (e->Direction() == 1 && Position().x > center.x) {
            angle -= PI / 2.f;
        } else if (e->Direction() == -1 && Position().x < center.x) {
            angle += PI / 2.f;
        }
    } else {
        if (e->Direction() == -1) angle -= PI / 2.f;
    }
    
    data.velocity_ = {cos(angle) * kb, sin(angle) * kb};
}
