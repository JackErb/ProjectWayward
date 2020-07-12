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

using std::list;
using std::cerr;
using std::cout;
using std::endl;

Character::Character(int id, const VectorV &vec) : Entity(id, vec) {
    initMslBindings();
	mslIntp->Init({ "NAIR", "UTILT", "UAIR", "NAIR", "FTILT", "DAIR", "JAB", "FAIR", "DTILT"});
    SetActionState(new AirborneNeutralState(this));
}

Character::~Character() {
    delete state;
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
        else { f = FixedPoint::FromFloat(mslIntp->getFloatParam(0)); }
        ApplyGravity(f);
    };
    
    mslIntp->bindings_["quit"] = [=]() {
        switch (state->GetState()) {
            case GROUNDED:
                SetActionState(new NeutralState(this));
                break;
            case AIRBORNE:
                SetActionState(new AirborneNeutralState(this));
                break;
        }
    };
    
    mslIntp->bindings_["setState"] = [=]() {
        std::string str = mslIntp->getStrParam(0);
        if (str.compare("LandingLag") == 0) {
            int f = mslIntp->getIntParam(1);
            SetActionState(new LandingLagState(this, f));
        } else if (str.compare("AirborneNeutralState") == 0) {
            SetActionState(new AirborneNeutralState(this));
        }
    };
}

void Character::ProcessInput(const PlayerInput &in) {
    if (Entity::data.freeze) {
        // Buffer ?
        return;
    }
    
    input = const_cast<PlayerInput*>(&in);
    state->ProcessInput(in);
}

void Character::Tick() {
	/*fpoat hyp = input_->stick.hyp();
	if (hyp > StickDZ::DEADZONE) {
		fpoat x = fpcos(hyp) * input_->stick.x / fpoat(1,0);
		fpoat y = fpsin(hyp) * input_->stick.y / fpoat(1,0);
		Transform(VectorV(x * attr.MaxAirSpeed, y * attr.MaxAirSpeed));
	}*/
    if (Entity::data.freeze) {
        Entity::data.freezeFr--;
        if (Entity::data.freezeFr <= 0) {
            Entity::data.freeze = false;
        }
        return;
    }
    
    fill = false;
    // Teleport back above stage
    /*if (Position().y > 1900 || Position().y < -2600 || fpabs(Position().x) > 3200) {
        Respawn();
    }*/
    
    if (data.ftCount > 0)  {
        if (--data.ftCount == 0) data.ft = -1;
    }

    // Do state dependent actions
    switch (state->GetState()) {
        case AIRBORNE:
            break;
        case GROUNDED:
            if (!engine->CheckBoundingBoxAgainstPolys(this, data.groundedData.stage)) {
                state->SwitchState(AIRBORNE);
                initAirborneData();
            }
            break;
    }
    
    state->Tick();
}

void Character::RollbackTick() {
    Entity::RollbackTick();
    
    data.stateType = state->GetStateType();
    data.stateData = state->data;
    
    rollback = data;
}

void Character::Rollback() {
    Entity::Rollback();

    delete state;
    
    data = rollback;
    switch (data.stateType) {
        case LandingLag:
            state = new LandingLagState(this, true);
            break;
        case Neutral:
            state = new NeutralState(this, true);
            break;
        case CharStateType::Dash:
            state = new DashState(this, true);
            break;
        case Jumpsquat:
            state = new JumpsquatState(this, true);
            break;
        case CharStateType::Turnaround:
            state = new TurnaroundState(this, true);
            break;
        case AirborneNeutral:
            state = new AirborneNeutralState(this, true);
            break;
        case CharStateType::Airdodge:
            state = new AirdodgeState(this, true);
            break;
        case AirborneScript:
            state = new AirborneScriptState(this, true);
            break;
        case GroundedScript:
            state = new GroundedScriptState(this, true);
            break;
        case Hitlag:
            state = new HitlagState(this, true);
            break;
        case Hitstun:
            state = new HitstunState(this, true);
            break;
    }
    state->setData(data.stateData);
}

void Character::HandleCollision(const Entity &entity, const VectorV &pv) {
    if (entity.id == data.ft) {
        return;
    }
    
    if (entity.Type() == CHARACTER) {
        fill = true;
    }
    
    if (entity.Type() == STAGE) {
        // Apply the push vector to prevent overlap
        Transform(pv);
    }
    
    if (state->GetState() == AIRBORNE) {
        fpoat vy = Velocity().y;
        if (entity.Type() == STAGE) {            
            if (pv.x.n == 0 && pv.y.sign && !vy.sign && vy.n != 0) {
                // Land on the stage
                NullVelocityY();
                state->SwitchState(GROUNDED);
                data.groundedData.stage = dynamic_cast<const StageEntity*>(&entity);
                return;
            } else if (!pv.y.sign && pv.x.n == 0) {
                NullVelocityY();
            }
        } else if (entity.Type() == PLATFORM && !input->stick.inDirection(DOWN)) {
            if (pv.x.n == 0 && pv.y.sign) {
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
                    
                    state->SwitchState(GROUNDED);
                    data.groundedData.stage = dynamic_cast<const StageEntity*>(&entity);
                    return;
				}
			}
        }
    }
    
    state->HandleCollision(entity, pv);
}

bool Character::HandleHit(const Entity *e, int f, const HitboxData &hd) {
    data.percent = data.percent + hd.dmg;
    
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
