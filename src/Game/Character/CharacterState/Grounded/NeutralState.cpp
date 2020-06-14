//
//  NeutralState.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/26/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "NeutralState.hpp"
#include "../Airborne/AirborneNeutralState.hpp"
#include "DashState.hpp"
#include "JumpsquatState.hpp"
#include "GroundedScriptState.hpp"
#include "../../Character.hpp"
#include "../../../Entities/Entity.hpp"


void NeutralState::NullVelocity() {
    character_->NullVelocityY();
}

void NeutralState::ProcessInput(const PlayerInput &input) {
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
            std::cout << "HERE" << std::endl;
        }
        return;
    }
    
    if (input.IsPressed(0)) {
        character_->SetActionState(new GroundedScriptState(character_, "FSPECIAL"));
        return;
    }
    
    float hyp = input.stick.hyp();
    if (hyp >= PlayerInput::DEAD_ZONE) {
        if (input.stick.inDirection(DOWN_T) &&
            character_->Stage()->Type() == EntityType::PLATFORM) {
            // Fall through platform
            character_->FallthroughPlatform();
            character_->SetActionState(new AirborneNeutralState(character_));
            return;
        } else {
            character_->SetActionState(new DashState(character_));
            return;
        }
    }
}

void NeutralState::Tick() {    
    character_->ApplyFriction();
    character_->ApplyVelocity();
    //character_->SetSprite(character_->GetSprite("dash", data.frame_ % 11));
}

void NeutralState::HandleCollision(const Entity &entity, sf::Vector2f pv) {
}

void NeutralState::SwitchState(CharState state) {
    if (state == AIRBORNE) {
        character_->SetActionState(new AirborneNeutralState(character_));
    } else {
        std::cerr << "ERROR SWITCHING STATES" << std::endl;
    }
}
