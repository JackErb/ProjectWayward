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
    data.frame--;
    character->ApplyFriction();
    character->ApplyVelocity();
    if (data.frame == 0) {
        character->SetActionState(new NeutralState(character));
        return;
    }
}

void LandingLagState::HandleCollision(const Entity &e1, VectorV pv) {
    
}

void LandingLagState::SwitchState(CharState state) {
    if (state == AIRBORNE) {
        character->SetActionState(new AirborneNeutralState(character));
        return;
    } else {
        std::cerr << "ERROR SWITCHING STATES IN LANDING LAG" << std::endl;
    }
}
