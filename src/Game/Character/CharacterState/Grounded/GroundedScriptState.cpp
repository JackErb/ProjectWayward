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
    data.frame_ = 0;
    character_->move = m;
    character_->mslIntp->InitScript(m);
}

void GroundedScriptState::ProcessInput(const PlayerInput &input) {
    data.frame_++;
    character_->mslIntp->PreTick(data.frame_);
    character_->mslIntp->ProcessInput();
}

void GroundedScriptState::Tick() {
    character_->mslIntp->Tick();
}

void GroundedScriptState::HandleCollision(const Entity &entity, sf::Vector2f pv) {

}

void GroundedScriptState::SwitchState(CharState state) {
    if (state == AIRBORNE) {
        character_->mslIntp->CallFunction("Airborne");
    }
}
