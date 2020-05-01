//
//  TurnaroundState.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "TurnaroundState.hpp"
#include "DashState.hpp"
#include "JumpsquatState.hpp"
#include "AirborneNeutralState.hpp"

void TurnaroundState::ProcessInput(const PlayerInput &input) {
    if (input.IsPressed(3)) {
        character_->SetActionState(new JumpsquatState(character_));
        return;
    }
}

void TurnaroundState::Tick() {
    frame_--;
    if (frame_ == 0) {
        PlayerInput::StickState stick = character_->input_->stick;
        character_->SetActionState(new DashState(character_, stick.angle(), stick.xAxis));
        return;
    }
}

void TurnaroundState::HandleCollision(const Entity &e, sf::Vector2f pv) {
    
}

void TurnaroundState::SwitchState(Character::CState state) {
    switch (state) {
        case Character::AIRBORNE:
            character_->SetActionState(new AirborneNeutralState(character_));
            return;
        case Character::GROUNDED:
            std::cerr << "ERROR SWITCH STATE TURNAROUNDSTATE" << std::endl;
            return;
    }
}
