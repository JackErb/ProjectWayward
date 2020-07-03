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
#include "CharacterState/Grounded/GroundedScriptState.hpp"

#include "CharacterState/Airborne/AirdodgeState.hpp"
#include "CharacterState/Airborne/HitlagState.hpp"
#include "CharacterState/Airborne/AirborneNeutralState.hpp"
#include "CharacterState/Airborne/AirborneScriptState.hpp"
#include "CharacterState/Airborne/HitlagState.hpp"
#include "CharacterState/Airborne/HitstunState.hpp"

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

Character::Character(int id, const VectorV &vec) : Entity(id, vec) {
    initMslBindings();
	mslIntp->Init({ "NAIR" });// {"UTILT", "UAIR", "NAIR", "FTILT", "DAIR", "JAB", "FAIR", "DTILT"});
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
        fpoat f;
        if (mslIntp->numParams() == 0) { f = fpoat(1,0); }
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
	/*fpoat hyp = input_->stick.hyp();
	if (hyp > StickDZ::DEADZONE) {
		fpoat x = fpcos(hyp) * input_->stick.x / fpoat(1,0);
		fpoat y = fpsin(hyp) * input_->stick.y / fpoat(1,0);
		Transform(VectorV(x * attr.MaxAirSpeed, y * attr.MaxAirSpeed));
	}*/
    if (Entity::data.freeze_) {
        Entity::data.freezeFr_--;
        if (Entity::data.freezeFr_ <= 0) {
            Entity::data.freeze_ = false;
        }
        return;
    }
    
    fill = false;
    // Teleport back above stage
    if (Position().y > 1900 || Position().y < -2600 || fpabs(Position().x) > 3200) {
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
            actionState_ = new NeutralState(this, true);
            break;
        case CharStateType::Dash:
            actionState_ = new DashState(this, true);
            break;
        case Jumpsquat:
            actionState_ = new JumpsquatState(this, true);
            break;
        case CharStateType::Turnaround:
            actionState_ = new TurnaroundState(this, true);
            break;
        case AirborneNeutral:
            actionState_ = new AirborneNeutralState(this, true);
            break;
        case CharStateType::Airdodge:
            actionState_ = new AirdodgeState(this, true);
            break;
        case AirborneScript:
            actionState_ = new AirborneScriptState(this, true);
            break;
        case GroundedScript:
            actionState_ = new GroundedScriptState(this, true);
            break;
        case Hitlag:
            actionState_ = new HitlagState(this, true);
            break;
        case Hitstun:
            actionState_ = new HitstunState(this, true);
            break;
    }
    actionState_->setData(data.actionStateData_);
}

void Character::HandleCollision(const Entity &entity, const VectorV &pv) {
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
        fpoat vy = Velocity().y;
        if (entity.Type() == STAGE) {            
            if (pv.x.n == 0 && pv.y.sign && !vy.sign) {
                // Land on the stage
                NullVelocityY();
                actionState_->SwitchState(GROUNDED);
                data.groundedData.stage = dynamic_cast<const StageEntity*>(&entity);
                return;
            }
        } else if (entity.Type() == PLATFORM && !input_->stick.inDirection(DOWN)) {
            if (pv.y.sign) {
                // The character collided with the platform. Check if the character
                // is above the platform and falling down
                Rectangle b = BoundingBox();
                Rectangle s = entity.BoundingBox();
                
                bool vert_check = (b.y + b.h) - vy * fpoat(1,1) < s.y;
                bool horiz_check = (b.x + b.w * fpoat(0,7)) > s.x && (b.x + b.w * fpoat(0,3)) < (s.x + s.w);
				if (!vy.sign && vert_check && horiz_check) {
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
    data.percent_ = data.percent_ + hd.dmg;
    
    // Calculate angle
    fpoat angle = hd.angle;
    if (hd.reverse) {
        VectorV center = geometric_center(hd.hitbox);
        center.x = center.x * e->Direction();
        center = center + e->Position();
        
        if (Position().x < center.x) {
            angle = angle - FixedPoint::PI * fpoat(0,5000);
        }
    } else {
        if (e->Direction() == -1) angle = FixedPoint::PI - angle;
    }
    
    SetActionState(new HitlagState(this, f, angle, hd.basekb, hd.kbscale));
    return true;
}
