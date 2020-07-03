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

void Character::ApplyGravity(const fpoat& m) {
    if (data.airborneData.fastfall) {
        FastFall();
        return;
    }
    Entity::data.velocity_.y = Entity::data.velocity_.y + m * attr.Gravity;
    if (actionState_->GetStateType() != Hitstun) {
        Entity::data.velocity_.y = fpmin(attr.MaxFallSpeed, Entity::data.velocity_.y);
    }
}

void Character::Jump(JumpType type, bool fullhop) {
    if (actionState_->GetState() != AIRBORNE) {
        std::cerr << "ERROR: JUMP INVALID STATE " << std::endl;
        return;
    }
    
    if (data.airborneData.fastfall) {
        data.airborneData.fastfall = false;
    }
    
    fpoat xv;
	fpoat yv = fullhop ? attr.FullhopJump : attr.ShorthopJump;
    if (type == JLEFT || type == JUP || type == JRIGHT) {
        initAirborneData();
    }
    switch (type) {
        case JLEFT:
            xv = - attr.MaxGroundSpeed;
            break;
        case JUP:
            xv = fpoat(0);
            break;
        case JRIGHT:
            xv = attr.MaxGroundSpeed;
            break;
        case DJUMP:
            if (data.airborneData.jumps > 0) {
                data.airborneData.jumps--;
                xv = fpoat(0);
                yv = attr.DoubleJump;
            } else {
                // No jumps left
                return;
            }
            break;
    }
    SetActionState(new AirborneNeutralState(this));
    Entity::data.velocity_.y = yv;
    Entity::data.velocity_.x = xv;
}

void Character::Dash(const fpoat &m) {
    if (actionState_->GetState() != GROUNDED) {
        std::cerr << "ERROR: DASH INVALID STATE " << std::endl;
        return;
    }
    
    if (fpabs(m) > fpoat(1,0)) {
        std::cerr << "INVALID DASH MODIFIER" << std::endl;
        return;
    }
    
	Entity::data.velocity_.x = m * attr.MaxGroundSpeed;
}

void Character::Vector() {
    if (actionState_->GetState() != AIRBORNE) {
        std::cerr << "ERROR: VECTOR INVALID STATE " << std::endl;
        return;
    }
    
    fpoat hyp = input_->stick.hyp();
    if (hyp <= StickDZ::DEADZONE) {
        ApplyFriction();
        return;
    }
    
    fpoat x = fpabs(input_->stick.x);
    fpoat m = x > fpoat(0,7) ? fpoat(1,0) : x * fpoat(1,4286); // This arbitrary fpoat is 1 / 0.7
	m.sign = input_->stick.x.sign;
    
    Entity::data.velocity_.x = Entity::data.velocity_.x + m * attr.AirAccel;
	bool sign = Entity::data.velocity_.x.sign;

	// Cap air speed
    Entity::data.velocity_.x = fpmin(attr.MaxAirSpeed, fpabs(Entity::data.velocity_.x));
	Entity::data.velocity_.x.sign = sign;
    
    if (input_->stick.inDirection(DOWN_T) && hyp > StickDZ::HIGHRING && !Entity::data.velocity_.y.sign) {
        FastFall();
    }
}

void Character::FastFall() {
    Entity::data.velocity_.y = attr.FastFallSpeed;
    data.airborneData.fastfall = true;
}

void Character::ApplyFriction() {
	fpoat m = (actionState_->GetState() == GROUNDED) ? attr.GroundFriction : attr.AirFriction;
    Entity::data.velocity_.x = Entity::data.velocity_.x * m;
}

void Character::SetActionState(CharacterState *s) {
    Entity::data.ignoreHits.clear();
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
        SetVelocity(dir * attr.MaxAirSpeed, attr.WallJump);
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
    
	fpoat a = input_->stick.angle();
    SetVelocity(fpcos(a) * attr.AirdodgeVelocity, - fpsin(a) * attr.AirdodgeVelocity);
    data.airborneData.airdodge = false;
}
