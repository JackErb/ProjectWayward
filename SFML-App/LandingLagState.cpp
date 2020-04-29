//
//  LandingLagState.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/27/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "LandingLagState.hpp"
#include "NeutralState.hpp"

void LandingLagState::ProcessInput(const PlayerInput &input) {
    
}

void LandingLagState::Tick() {
    frames_--;
    character_->ApplyFriction();
    character_->ApplyVelocity();
    if (frames_ == 0) {
        character_->SetActionState(new NeutralState(character_));
        return;
    }
}

void LandingLagState::HandleCollision(const Entity &e1, sf::Vector2f pv) {
    
}
