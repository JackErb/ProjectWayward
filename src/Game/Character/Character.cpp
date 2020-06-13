//
//  Character.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/26/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "Character.hpp"

#include "CharacterState/Grounded/NeutralState.hpp"
#include "CharacterState/Grounded/LandingLagState.hpp"
#include "CharacterState/Grounded/DashState.hpp"
#include "CharacterState/Grounded/TurnaroundState.hpp"
#include "CharacterState/Grounded/JumpsquatState.hpp"
#include "CharacterState/Airborne/AirdodgeState.hpp"
#include "CharacterState/Airborne/HitlagState.hpp"
#include "CharacterState/Airborne/AirborneNeutralState.hpp"

#include "../Physics/PhysicsEngine.hpp"
#include "../Entities/StageEntity.hpp"
#include "../Entities/PlatformEntity.hpp"
#include "../Loaders/MoveLoader.hpp"

#include "../../MSL/MslInterpreter.hpp"
#include "../../MSL/MslScanner.hpp"
#include "../MathHelper.hpp"

using std::list;
using std::cerr;
using std::cout;
using std::endl;

Character::Character(int id, sf::Vector2f vec) : Entity(id, vec) {
    initMslBindings();
    mslIntp->Init({"UTILT", "UAIR", "NAIR", "FTILT", "DAIR", "JAB", "FAIR", "DTILT"});
    SetActionState(new AirborneNeutralState(this));
}

Character::~Character() {
    delete actionState_;
}

void Character::initMslBindings() {
    // Set up character script bindings
    mslIntp->bindings_["NullVelocity"] = [=]() {
        NullVelocityX();
        NullVelocityY();
    };
    
    mslIntp->bindings_["vector"] = [=]() {
        Vector();
    };
    
    mslIntp->bindings_["gravity"] = [=]() {
        float f;
        if (mslIntp->numParams() == 0) { f = 1.f; }
        else { f = mslIntp->getIntParam(0); }
        ApplyGravity(f);
    };
    
    mslIntp->bindings_["quit"] = [=]() {
        switch (actionState_->GetState()) {
            case GROUNDED:
                SetActionState(new NeutralState(this));
                break;
            case AIRBORNE:
                SetActionState(new AirborneNeutralState(this));
                break;
        }
    };
    
    mslIntp->bindings_["setState"] = [=]() {
        std::string state = mslIntp->getStrParam(0);
        if (state.compare("LandingLag") == 0) {
            int f = mslIntp->getIntParam(1);
            SetActionState(new LandingLagState(this, f));
        } else if (state.compare("AirborneNeutralState") == 0) {
            SetActionState(new AirborneNeutralState(this));
        }
    };
}

void Character::ProcessInput(const PlayerInput &input) {
    if (Entity::data.freeze_) {
        // Buffer ?
        return;
    }
    
    input_ = const_cast<PlayerInput* >(&input);
    actionState_->ProcessInput(input);
}

void Character::Tick() {
    if (Entity::data.freeze_) {
        Entity::data.freezeFr_--;
        if (Entity::data.freezeFr_ <= 0) {
            Entity::data.freeze_ = false;
        }
        return;
    }
    
    fill = false;
    // Teleport back above stage
    if (Position().y > 1900 || Position().y < -2600 || abs(Position().x) > 3200) {
        Respawn();
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
        float vy = Velocity().y;
        if (entity.Type() == STAGE) {            
            if (pv.x == 0 && pv.y < 0 && vy > 0) {
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
                
                bool vert_check = (b.y + b.h) - vy* 1.1f < s.y;
                bool horiz_check = (b.x + b.w * 5.f / 8.f) > s.x && (b.x + b.w * 3.f / 8.f) < (s.x + s.w);
                if (vy > 0 && vert_check && horiz_check) {
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

bool Character::HandleHit(const Entity *e, int f, const HitboxData &hd) {
    data.percent_ += hd.dmg;
    
    // Calculate angle
    float angle = hd.angle;
    if (hd.reverse) {
        sf::Vector2f center = geometric_center(hd.hitbox);
        center.x *= e->Direction();
        center += e->Position();
        
        if (Position().x < center.x) {
            angle -= PI / 2.f;
        }
    } else {
        if (e->Direction() == -1) angle = PI - angle;
    }
    
    SetActionState(new HitlagState(this, f, angle, hd.basekb, hd.kbscale));
    return true;
}
