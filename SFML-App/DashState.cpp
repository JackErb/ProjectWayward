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
#include "AirborneNeutralState.hpp"
#include "JumpsquatState.hpp"

void DashState::setDirInfluence(float a, float x) {
    if (fabs(x) > PlayerInput::DEAD_ZONE) {
        if (PlayerInput::InRange(a, 1.f / 3.f * M_PI, 2.f / 3.f * M_PI)
            || PlayerInput::InRange(a, -2.f / 3.f * M_PI, -1.f / 3.f * M_PI)) {
            // Walk
            
            dirInfluence_ = x / 60.f;
        } else {
            dirInfluence_ = 1.f;
            if (a >= M_PI / 2.f || a <= - M_PI / 2.f) dirInfluence_ *= -1;
        }
    } else {
        dirInfluence_ = 0.f;
    }
}

void DashState::ProcessInput(const PlayerInput &input) {
    if (input.IsPressed(3)) {
        character_->SetActionState(new JumpsquatState(character_));
        return;
    }
    
    if (input.stick.hyp() < PlayerInput::DEAD_ZONE) {
        character_->SetActionState(new NeutralState(character_));
        return;
    }
    
    setDirInfluence(input.stick.angle(), input.stick.xAxis);
}

void DashState::Tick() {
    if (abs(dirInfluence_) > 0.f) {
        character_->Dash(dirInfluence_);
    }
    character_->ApplyVelocity();
}

void DashState::HandleCollision(const Entity &e1, sf::Vector2f pv) {
    
}

void DashState::SwitchState(Character::CState state) {
    switch (state) {
        case Character::GROUNDED:
            std::cerr << "Switch to grounded state while actionState=Dash" << std::endl;
            return;
        case Character::AIRBORNE:
            character_->SetActionState(new AirborneNeutralState(character_));
            return;
    }
}
