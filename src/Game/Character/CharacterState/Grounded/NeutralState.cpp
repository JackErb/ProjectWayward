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
    character->NullVelocityY();
}

void NeutralState::ProcessInput(const PlayerInput &input) {
    if (input.IsPressed(JUMP)) {
        character->SetActionState(new JumpsquatState(character));
        return;
    }
    
    if (input.IsPressed(ATTACK)) {
        if (input.stick.inDirection(UP_T)) {
            character->SetActionState(new GroundedScriptState(character, "UTILT"));
        } else if (input.stick.inDirection(LEFT_T)) {
            if (character->Direction() == 1) {
                character->Turnaround();
            }
            character->SetActionState(new GroundedScriptState(character, "FTILT"));
        } else if (input.stick.inDirection(RIGHT_T)) {
            if (character->Direction() == -1) {
                character->Turnaround();
            }
            character->SetActionState(new GroundedScriptState(character, "FTILT"));
        } else if (input.stick.inDirection(DOWN_T)) {
            character->SetActionState(new GroundedScriptState(character, "DTILT"));
        } else {
            character->SetActionState(new GroundedScriptState(character, "JAB"));
            std::cout << "HERE" << std::endl;
        }
        return;
    }
    
    if (input.IsPressed(SPECIAL)) {
        character->SetActionState(new GroundedScriptState(character, "FSPECIAL"));
        return;
    }
    
    fpoat hyp = input.stick.hyp();
    if (hyp >= StickDZ::DEADZONE) {
        if (input.stick.inDirection(DOWN_T) &&
            character->Stage()->Type() == Ent_Platform) {
            // Fall through platform
            character->FallthroughPlatform();
            character->SetActionState(new AirborneNeutralState(character));
            return;
        } else {
            character->SetActionState(new DashState(character));
            return;
        }
    }
}

void NeutralState::Tick() {    
    character->ApplyFriction();
    character->ApplyVelocity();
    //character->SetSprite(character->GetSprite("dash", data.frame_ % 11));
}

void NeutralState::HandleCollision(const Entity &entity, VectorV pv) {
}

void NeutralState::SwitchState(CharState state) {
    if (state == State_Airborne) {
        character->SetActionState(new AirborneNeutralState(character));
    } else {
        std::cerr << "ERROR SWITCHING STATES" << std::endl;
    }
}
