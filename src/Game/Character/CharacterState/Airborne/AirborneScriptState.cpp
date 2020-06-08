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
    data.frame_ = 0;
    character_->mslIntp->InitScript(m);
}

void AirborneScriptState::ProcessInput(const PlayerInput &input) {
    data.frame_++;
    character_->mslIntp->PreTick(data.frame_);
    character_->mslIntp->ProcessInput();
}

void AirborneScriptState::Tick() {
    character_->mslIntp->Tick();
    character_->ApplyVelocity();
}

void AirborneScriptState::HandleCollision(const Entity &entity, sf::Vector2f pv) {

}

void AirborneScriptState::SwitchState(CharState state) {
    if (state == GROUNDED) {
        character_->mslIntp->CallFunction("Land");
    }
}
