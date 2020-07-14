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
    Entity::data.velocity.y = Entity::data.velocity.y + m * attr.Gravity;
    if (state->GetStateType() != Hitstun) {
        Entity::data.velocity.y = fpmin(attr.MaxFallSpeed, Entity::data.velocity.y);
    }
}

void Character::Jump(JumpType type, bool fullhop) {
    if (state->GetState() != State_Airborne) {
        std::cerr << "ERROR: JUMP INVALID STATE " << std::endl;
        return;
    }
    
    if (data.airborneData.fastfall) {
        data.airborneData.fastfall = false;
    }
    
    fpoat xv;
	fpoat yv = fullhop ? attr.FullhopJump : attr.ShorthopJump;
    if (type == Jump_Left || type == Jump_Up || type == Jump_Right) {
        initAirborneData();
    }
    switch (type) {
        case Jump_Left:
            xv = - attr.MaxGroundSpeed;
            break;
        case Jump_Up:
            xv = fpoat(0);
            break;
        case Jump_Right:
            xv = attr.MaxGroundSpeed;
            break;
        case Jump_Double:
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
    Entity::data.velocity.y = yv;
    Entity::data.velocity.x = xv;
}

void Character::Dash(const fpoat &m) {
    if (state->GetState() != State_Grounded) {
        std::cerr << "ERROR: DASH INVALID STATE " << std::endl;
        return;
    }
    
    if (fpabs(m) > fpoat(1,0)) {
        std::cerr << "INVALID DASH MODIFIER" << std::endl;
        return;
    }
    
	Entity::data.velocity.x = m * attr.MaxGroundSpeed;
}

void Character::Vector() {
    if (state->GetState() != State_Airborne) {
        std::cerr << "ERROR: VECTOR INVALID STATE " << std::endl;
        return;
    }
    
    fpoat hyp = input->stick.hyp();
    if (hyp <= StickDZ::DEADZONE) {
        ApplyFriction();
        return;
    }
    
    fpoat x = fpabs(input->stick.x);
    fpoat m = x > fpoat(0,7) ? fpoat(1,0) : x * fpoat(1,4286); // This arbitrary fpoat is 1 / 0.7
	m.sign = input->stick.x.sign;
    
    Entity::data.velocity.x = Entity::data.velocity.x + m * attr.AirAccel;
	bool sign = Entity::data.velocity.x.sign;

	// Cap air speed
    Entity::data.velocity.x = fpmin(attr.MaxAirSpeed, fpabs(Entity::data.velocity.x));
	Entity::data.velocity.x.sign = sign;
    
    if (input->stick.inDirection(DOWN_T) && hyp > StickDZ::HIGHRING && !Entity::data.velocity.y.sign) {
        FastFall();
    }
}

void Character::FastFall() {
    Entity::data.velocity.y = attr.FastFallSpeed;
    data.airborneData.fastfall = true;
}

void Character::ApplyFriction() {
	fpoat m = (state->GetState() == State_Grounded) ? attr.GroundFriction : attr.AirFriction;
    Entity::data.velocity.x = Entity::data.velocity.x * m;
}

void Character::SetActionState(CharacterState *s) {
    Entity::data.ignoreHits.clear();
    data.stateType = s->GetStateType();
    state = s;
}

void Character::FallthroughPlatform() {
    if (state->GetState() == State_Airborne ||
        data.groundedData.stage->Type() != Ent_Platform) {
        std::cerr << "ERROR Attempt to fall through " << data.groundedData.stage->Type();
        std::cerr << ", WITH STATE " << state->GetState() << std::endl;
        return;
    }
    
    data.ft = data.groundedData.stage->id;
    data.ftCount = 4;
    initAirborneData();
}

void Character::WallJump(int dir) {
    if (state->GetState() != State_Airborne) {
        std::cerr << "ERROR Attempt to walljump, state: " << state->GetState() << std::endl;
        return;
    }
    
    if (data.airborneData.walljump) {
        SetVelocity(dir * attr.MaxAirSpeed, attr.WallJump);
        data.airborneData.walljump = false;
    }
}

void Character::Airdodge() {
    if (state->GetState() != State_Airborne || !data.airborneData.airdodge) {
        std::cerr << "ERROR Attempt to airdodge, state: " << state->GetState();
        std::cerr << ", airdodge: " << data.airborneData.airdodge << std::endl;
        return;
    }
    
	fpoat a = input->stick.angle();
    SetVelocity(fpcos(a) * attr.AirdodgeVelocity, - fpsin(a) * attr.AirdodgeVelocity);
    data.airborneData.airdodge = false;
}
