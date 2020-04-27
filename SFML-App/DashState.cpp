//
//  DashState.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/27/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "DashState.hpp"
#include "PlayerInput.hpp"
#include "NeutralState.hpp"

void DashState::setDirInfluence(float a) {    
    if (PlayerInput::InRange(a, 1.f / 3.f * M_PI, 2.f / 3.f * M_PI)
        || PlayerInput::InRange(a, -1.f / 3.f * M_PI, -2.f / 3.f * M_PI)) {
        // Walk
        
        // m is constrained to [0, 1/6 * pi]
        float m = abs(abs(a) - 1.f / 2.f);
        dirInfluence_ = m / (1.f / 6.f * M_PI);
    } else {
        dirInfluence_ = 1;
    }
    if (a >= M_PI / 2.f || a <= - M_PI / 2.f) dirInfluence_ *= -1;
}

void DashState::ProcessInput(const PlayerInput &input) {
    setDirInfluence(input.stick.angle());
}

void DashState::Tick() {
    character_->Dash(dirInfluence_);
    character_->ApplyVelocity();
}

void DashState::HandleCollision(const Entity &e1, sf::Vector2f pv) {
    
}

void DashState::SwitchState(State state) {
    switch (state) {
        case CharacterState::GROUNDED:
            std::cerr << "Switch to grounded state while actionState=Dash" << std::endl;
            return;
        case CharacterState::AIRBORNE:
            character_->SetActionState(new NeutralState(character_));
            return;
    }
}
