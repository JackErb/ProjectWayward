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
#include "Character.hpp"
#include "Entity.hpp"

#define PI 3.14159

void NeutralState::NullVelocity() {
    character_->NullVelocityY();
}

void NeutralState::ProcessInput(const PlayerInput &input) {
    if (input.IsPressed(3)) {
        character_->SetActionState(new JumpsquatState(character_));
        return;
    }
    
    if (input.IsPressed(2)) {
        data.frame_++;
    }
    
    float hyp = input.stick.hyp();
    if (hyp >= PlayerInput::DEAD_ZONE) {
        if (input.stick.inDirection(DOWN_T) &&
            character_->Stage()->Type() == EntityType::PLATFORM) {
            // Fall through platform
            character_->FallthroughPlatform();
            character_->SetActionState(new AirborneNeutralState(character_));
            return;
        } else {
            character_->SetActionState(new DashState(character_));
            return;
        }
    }
}

void NeutralState::Tick() {    
    character_->ApplyFriction();
    character_->ApplyVelocity();
    character_->SetSprite(character_->GetSprite("dash", data.frame_ % 11));
}

void NeutralState::HandleCollision(const Entity &entity, sf::Vector2f pv) {
}

void NeutralState::SwitchState(CharState state) {
    if (state == AIRBORNE) {
        character_->SetActionState(new AirborneNeutralState(character_));
    } else {
        std::cerr << "ERROR SWITCHING STATES" << std::endl;
    }
}