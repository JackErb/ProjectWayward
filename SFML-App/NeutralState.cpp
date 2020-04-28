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
    float hyp = input.stick.hyp();
    if (hyp >= DEAD_ZONE) {
        character_->SetActionState(new DashState(character_, input.stick.angle()));
        return;
    }
    
    if (input.IsPressed(3)) {
        character_->SetActionState(new JumpsquatState(character_));
        return;
    }
}

void NeutralState::Tick() {
    character_->ApplyFriction();
}

void NeutralState::HandleCollision(const Entity &entity, sf::Vector2f pv) {
}

void NeutralState::SwitchState(State state) {
    if (state == AIRBORNE) {
        character_->SetActionState(new AirborneNeutralState(character_));
        return;
    } else {
        std::cerr << "ERROR SWITCHING STATES" << std::endl;
    }
}
