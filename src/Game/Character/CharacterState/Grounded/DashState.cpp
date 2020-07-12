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
	character->SetDirection(data.dirInfluence.sign ? -1 : 1);
}

bool DashState::setDirInfluence() {
    const PlayerInput *in = character->input;
    if (in->stick.hyp() > StickDZ::DEADZONE) {
        fpoat old = data.dirInfluence;
		data.dirInfluence = in->stick.x;
        if (old.sign != data.dirInfluence.sign) {
            return false;
        }
    } else {
        data.dirInfluence = FixedPoint::ZERO;
    }
            
    return true;
}

void DashState::ProcessInput(const PlayerInput &input) {
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
        }
        return;
    }
    
    if (input.IsPressed(SPECIAL)) {
        character->SetActionState(new GroundedScriptState(character, "FSPECIAL"));
        return;
    }
    
    if (input.stick.hyp() < StickDZ::DEADZONE) {
        character->SetActionState(new NeutralState(character));
        return;
    }
    
    bool res = setDirInfluence();
    if (!res) {
        // The player inputted the opposite direction. Only turnaround
        // If the hyp is not super small
        character->SetActionState(new TurnaroundState(character));
    }
}

void DashState::Tick() {
    character->SetTexture(character->GetTexture("dash", (data.frame / 4) % 11));
    data.frame++;
                          
    character->Dash(data.dirInfluence);
    character->ApplyVelocity();
}

void DashState::HandleCollision(const Entity &e1, VectorV pv) {
    
}

void DashState::SwitchState(CharState state) {
    switch (state) {
        case GROUNDED:
            std::cerr << "Switch to grounded state while actionState=Dash" << std::endl;
            return;
        case AIRBORNE:
            character->SetActionState(new AirborneNeutralState(character));
            return;
    }
}
