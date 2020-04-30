//
//  NeutralState.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/26/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "NeutralState.hpp"
#include "AirborneNeutralState.hpp"
#include "DashState.hpp"
#include "JumpsquatState.hpp"

#define PI 3.14159

void NeutralState::ProcessInput(const PlayerInput &input) {
    if (input.IsPressed(3)) {
        character_->SetActionState(new JumpsquatState(character_));
        return;
    }
    
    if (input.IsPressed(2)) {
        frame_++;
        std::cout << frame_ % 11 << std::endl;
    }
    
    float hyp = input.stick.hyp();
    if (hyp >= PlayerInput::DEAD_ZONE) {
        if (input.stick.inDirection(DOWN_T) &&
            character_->groundedData.stage->Type() == EntityType::PLATFORM) {
            // Fall through platform
            character_->FallthroughPlatform();
            character_->SetActionState(new AirborneNeutralState(character_));
            return;
        } else {
            character_->SetActionState(new DashState(character_, input.stick.angle(), input.stick.xAxis));
            return;
        }
    }
}

void NeutralState::Tick() {
    character_->ApplyFriction();
    character_->ApplyVelocity();
    character_->SetSprite(character_->GetSprite("dash", frame_ % 11));
}

void NeutralState::HandleCollision(const Entity &entity, sf::Vector2f pv) {
}

void NeutralState::SwitchState(Character::CState state) {
    if (state == Character::AIRBORNE) {
        character_->SetActionState(new AirborneNeutralState(character_));
        return;
    } else {
        std::cerr << "ERROR SWITCHING STATES" << std::endl;
    }
}
