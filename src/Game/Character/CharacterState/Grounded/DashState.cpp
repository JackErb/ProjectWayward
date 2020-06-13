//
//  DashState.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/27/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "DashState.hpp"
#include "../../../PlayerInput.hpp"
#include "NeutralState.hpp"
#include "../Airborne/AirborneNeutralState.hpp"
#include "JumpsquatState.hpp"
#include "TurnaroundState.hpp"
#include "../../Character.hpp"
#include "GroundedScriptState.hpp"

int sign(float f) {
    return f < 0.f ? -1 : 1;
}

void DashState::init() {
    setDirInfluence();
}

bool DashState::setDirInfluence() {
    const PlayerInput *in = character_->input_;
    if (in->stickHyp() > PlayerInput::DEAD_ZONE) {
        float old = data.dirInfluence_;
        data.dirInfluence_ = clamp(in->stickX() / 55.f, -1.f, 1.f);
        
        if (sign(old) != sign(data.dirInfluence_)) {
            return false;
        }
    } else {
        data.dirInfluence_ = 0.f;
    }
            
    return true;
}

void DashState::ProcessInput(const PlayerInput &input) {
    if (input.IsPressed(3)) {
        character_->SetActionState(new JumpsquatState(character_));
        return;
    }
    
    if (input.IsPressed(1)) {
        if (input.stick.inDirection(UP_T)) {
            character_->SetActionState(new GroundedScriptState(character_, "UTILT"));
        } else if (input.stick.inDirection(LEFT_T)) {
            if (character_->Direction() == 1) {
                character_->Turnaround();
            }
            character_->SetActionState(new GroundedScriptState(character_, "FTILT"));
        } else if (input.stick.inDirection(RIGHT_T)) {
            if (character_->Direction() == -1) {
                character_->Turnaround();
            }
            character_->SetActionState(new GroundedScriptState(character_, "FTILT"));
        } else if (input.stick.inDirection(DOWN_T)) {
            character_->SetActionState(new GroundedScriptState(character_, "DTILT"));
        } else {
            character_->SetActionState(new GroundedScriptState(character_, "JAB"));
        }
        return;
    }
    
    if (input.IsPressed(0)) {
        character_->SetActionState(new GroundedScriptState(character_, "FSPECIAL"));
        return;
    }
    
    if (input.stick.hyp() < PlayerInput::DEAD_ZONE) {
        character_->SetActionState(new NeutralState(character_));
        return;
    }
    
    bool res = setDirInfluence();
    if (!res) {
        // The player inputted the opposite direction. Only turnaround
        // If the hyp is not super small
        character_->SetActionState(new TurnaroundState(character_));
    }
}

void DashState::Tick() {
    character_->SetSprite(character_->GetSprite("dash", (data.frame_ / 4) % 11));
    data.frame_++;
                          
    if (abs(data.dirInfluence_) > 0.f) {
        character_->Dash(data.dirInfluence_);
    }
    character_->ApplyVelocity();
}

void DashState::HandleCollision(const Entity &e1, sf::Vector2f pv) {
    
}

void DashState::SwitchState(CharState state) {
    switch (state) {
        case GROUNDED:
            std::cerr << "Switch to grounded state while actionState=Dash" << std::endl;
            return;
        case AIRBORNE:
            character_->SetActionState(new AirborneNeutralState(character_));
            return;
    }
}
