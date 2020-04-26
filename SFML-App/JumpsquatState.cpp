//
//  JumpsquatState.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/26/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "JumpsquatState.hpp"

void JumpsquatState::ProcessInput(const PlayerInput &input) {
}

void JumpsquatState::Tick() {
    if (stateFrame_ >= 3) {
        character_->SetState(Character::AIRBORNE);
        character_->Jump();
    }
    
    stateFrame_++;
}

void JumpsquatState::HandleCollision(const Entity &e1, sf::Vector2f pv) {
    
}
