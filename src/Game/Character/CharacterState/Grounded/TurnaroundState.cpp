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
#include "../Airborne/AirborneNeutralState.hpp"
#include "../../Character.hpp"

void TurnaroundState::Turnaround() {
    character->NullVelocityX();
    character->Turnaround();
}

void TurnaroundState::ProcessInput(const PlayerInput &input) {
    if (input.IsPressed(JUMP)) {
        character->SetActionState(new JumpsquatState(character));
        return;
    }
}

void TurnaroundState::Tick() {
    data.frame--;
    if (data.frame == 0) {
        character->SetActionState(new DashState(character));
        return;
    }
}

void TurnaroundState::HandleCollision(const Entity &e, VectorV pv) {
    
}

void TurnaroundState::SwitchState(CharState state) {
    switch (state) {
        case State_Airborne:
            character->SetActionState(new AirborneNeutralState(character));
            return;
        case State_Grounded:
            std::cerr << "ERROR SWITCH STATE TURNAROUNDSTATE" << std::endl;
            return;
    }
}
