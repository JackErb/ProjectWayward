//
//  JumpsquatState.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/26/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "JumpsquatState.hpp"
#include "AirborneNeutralState.hpp"

void JumpsquatState::ProcessInput(const PlayerInput &input) {
    if (input.stick.hyp() > 40.f) {
        float a = input.stick.angle();
        if ((PlayerInput::InRange(a, 7.f / 8.f * M_PI, M_PI)
             || PlayerInput::InRange(a, -7.f / 8.f * M_PI, -M_PI))
             && type == Character::UP) {
            type = Character::LEFT;
        } else if (PlayerInput::InRange(a, -1.f / 8.f * M_PI, 1.f / 8.f * M_PI)
                   && type == Character::UP) {
            type = Character::RIGHT;
        }
    }
    
    if (input.IsReleased(3)) {
        jumpHeld = false;
    }
}

void JumpsquatState::Tick() {
    if (stateFrame_ >= 3) {
        character_->SetActionState(new AirborneNeutralState(character_));
        character_->Jump(type, jumpHeld);
    }
    
    stateFrame_++;
}

void JumpsquatState::HandleCollision(const Entity &e1, sf::Vector2f pv) {
    
}
