//
//  Character_state.cpp
//  SFML-App
//
//  Created by Jack Erb on 6/5/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "Character.hpp"

#include "CharacterState/Grounded/NeutralState.hpp"
#include "CharacterState/Airborne/AirborneNeutralState.hpp"
#include "CharacterState/Grounded/LandingLagState.hpp"
#include "CharacterState/Grounded/DashState.hpp"
#include "CharacterState/Grounded/TurnaroundState.hpp"
#include "CharacterState/Grounded/JumpsquatState.hpp"
#include "CharacterState/Airborne/AirdodgeState.hpp"
#include "../Physics/PhysicsEngine.hpp"
#include "../Entities/StageEntity.hpp"
#include "../Entities/PlatformEntity.hpp"
#include "../Loaders/MoveLoader.hpp"

void Character::ApplyGravity(float m) {
    if (data.airborneData.fastfall) {
        FastFall();
        return;
    }
    data.velocity_.y += m * attr.Gravity;
    data.velocity_.y = fmin(attr.MaxFallSpeed, data.velocity_.y);
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

void Character::FastFall() {
    data.velocity_.y = attr.FastFallSpeed;
    data.airborneData.fastfall = true;
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
