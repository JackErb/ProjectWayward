//
//  LandingLagState.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/27/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "LandingLagState.hpp"
#include "NeutralState.hpp"
#include "../Airborne/AirborneNeutralState.hpp"
#include "../../Character.hpp"

void LandingLagState::ProcessInput(const PlayerInput &input) {
    
}

void LandingLagState::Tick() {
    data.frame_--;
    character_->ApplyFriction();
    character_->ApplyVelocity();
    if (data.frame_ == 0) {
        character_->SetActionState(new NeutralState(character_));
        return;
    }
}

void LandingLagState::HandleCollision(const Entity &e1, sf::Vector2f pv) {
    
}

void LandingLagState::SwitchState(CharState state) {
    if (state == AIRBORNE) {
        character_->SetActionState(new AirborneNeutralState(character_));
        return;
    } else {
        std::cerr << "ERROR SWITCHING STATES IN LANDING LAG" << std::endl;
    }
}
