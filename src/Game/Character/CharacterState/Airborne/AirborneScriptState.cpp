//
//  AirborneScripState.cpp
//  SFML-App
//
//  Created by Jack Erb on 6/1/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "AirborneScriptState.hpp"
#include "../../Character.hpp"
#include "../../../../MSL/MslInterpreter.hpp"

void AirborneScriptState::init(std::string m) {
    data.frame = 0;
    character->SetMoveScript(m);
    character->mslIntp->InitScript(m);
}

void AirborneScriptState::ProcessInput(const PlayerInput &input) {
    data.frame++;
    character->mslIntp->PreTick(data.frame);
    character->mslIntp->ProcessInput();
}

void AirborneScriptState::Tick() {
    character->mslIntp->Tick();
    character->ApplyVelocity();
}

void AirborneScriptState::HandleCollision(const Entity &entity, VectorV pv) {

}

void AirborneScriptState::SwitchState(CharState state) {
    if (state == State_Grounded) {
        character->mslIntp->CallFunction("Land");
    }
}
