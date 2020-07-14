//
//  GroundedScriptState.cpp
//  SFML-App
//
//  Created by Jack Erb on 6/6/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "GroundedScriptState.hpp"
#include "../../Character.hpp"
#include "./../../../../MSL/MslInterpreter.hpp"

void GroundedScriptState::init(std::string m) {
    data.frame = 0;
    character->SetMoveScript(m);
}

void GroundedScriptState::ProcessInput(const PlayerInput &input) {
    data.frame++;
    character->mslIntp->PreTick(data.frame);
    character->mslIntp->ProcessInput();
}

void GroundedScriptState::Tick() {
    character->mslIntp->Tick();
}

void GroundedScriptState::HandleCollision(const Entity &entity, VectorV pv) {

}

void GroundedScriptState::SwitchState(CharState state) {
    if (state == State_Airborne) {
        character->mslIntp->CallFunction("Airborne");
    }
}
