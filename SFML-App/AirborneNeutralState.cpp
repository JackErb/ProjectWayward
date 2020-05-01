//
//  AirborneState.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/27/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "AirborneNeutralState.hpp"
#include "NeutralState.hpp"
#include "LandingLagState.hpp"
#include "AirdodgeState.hpp"

void AirborneNeutralState::ProcessInput(const PlayerInput &input) {
    if (input.IsPressed(3) && character_->airborneData.jumps > 0) {
        character_->Jump(Character::DJUMP, true);
    }
    
    if (input.IsPressed(7) && character_->airborneData.airdodge) {
        character_->SetActionState(new AirdodgeState(character_, input.stick.angle()));
        return;
    }
    
    if (input.IsPressed(0) && input.stick.inDirection(UP_T)) {
        character_->UpB();
        return;
    }
    
    float a = input.stick.angle();
    float hyp = input.stick.hyp();
    if (input.stick.inDirection(DOWN_T) && hyp > 50.f && character_->Velocity().y > 0.f) {
        character_->FastFall();
    }
    
    if (hyp > PlayerInput::DEAD_ZONE) {
        float m = (abs(input.stick.xAxis) > 60.f ? 60.f : abs(input.stick.xAxis)) / 60.f;
        if (input.stick.xAxis < 0) m *= -1;
        character_->Vector(m);
    } else {
        character_->ApplyFriction();
    }
}

void AirborneNeutralState::Tick() {
    if (!character_->airborneData.fastfall) {
        character_->ApplyGravity();
    } else {
        character_->FastFall();
    }
    character_->ApplyVelocity();
}

void AirborneNeutralState::HandleCollision(const Entity &entity, sf::Vector2f pv) {
    if (entity.Type() == STAGE) {
        if (abs(pv.x) > 0 && pv.y == 0) {
            if (pv.x < 0 && character_->input_->stick.inDirection(Direction::LEFT_T)) {
                character_->WallJump(-1);
                character_->SetDirection(-1);
            } else if (pv.x > 0 && character_->input_->stick.inDirection(Direction::RIGHT_T)) {
                character_->WallJump(1);
                character_->SetDirection(1);
            }
        }
    }
}

void AirborneNeutralState::SwitchState(Character::CState s) {
    if (s == Character::AIRBORNE) {
        std::cerr << "ATTEMPT TO SWITCH TO AIRBORNE WHILE AIRBORNENEUTRAL" << std::endl;
    } else if (s == Character::GROUNDED) {
        // Land on stage/platform
        character_->NullVelocityX();
        character_->SetActionState(new LandingLagState(character_, 2));
        return;
    }
}
