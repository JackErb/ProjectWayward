//
//  AirborneState.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/27/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "AirborneNeutralState.hpp"
#include "../Grounded/NeutralState.hpp"
#include "../Grounded/LandingLagState.hpp"
#include "AirdodgeState.hpp"
#include "../../Character.hpp"
#include "AirborneScriptState.hpp"

void AirborneNeutralState::ProcessInput(const PlayerInput &input) {
    if (input.IsPressed(JUMP) && character_->Jumps() > 0) {
        character_->Jump(DJUMP, true);
    }
    
    if (input.IsPressed(SHIELD) && character_->HasAirdodge()) {
        character_->SetActionState(new AirdodgeState(character_, input.stick.angle()));
        return;
    }
    
    if (input.IsPressed(ATTACK)) {
        if (input.stick.inDirection(DOWN_T)) {
            character_->SetActionState(new AirborneScriptState(character_, "DAIR"));
        } else if (input.stick.inDirection(LEFT_T) || input.stick.inDirection(RIGHT_T)) {
            character_->SetActionState(new AirborneScriptState(character_, "FAIR"));
        } else if (input.stick.inDirection(UP_T)) {
            character_->SetActionState(new AirborneScriptState(character_, "UAIR"));
        } else {
            character_->SetActionState(new AirborneScriptState(character_, "NAIR"));
        }
        return;
    }
    
    if (input.IsPressed(SPECIAL)) {
        character_->SetActionState(new AirborneScriptState(character_, "FSPECIAL"));
        return;
    }
    
     character_->Vector();
}

void AirborneNeutralState::Tick() {
    character_->ApplyGravity();
    character_->ApplyVelocity();
}

void AirborneNeutralState::HandleCollision(const Entity &entity, VectorV pv) {
    if (entity.Type() == STAGE) {
        if (pv.x.n != 0 && pv.y.n == 0) {
            if (pv.x < 0 && character_->input_->stick.inDirection(Direction::RIGHT_T)
                && character_->input_->IsPressed(JUMP)) {
                character_->WallJump(-1);
                character_->SetDirection(-1);
            } else if (pv.x > 0 && character_->input_->stick.inDirection(Direction::LEFT_T)
                       && character_->input_->IsPressed(JUMP)) {
                character_->WallJump(1);
                character_->SetDirection(1);
            }
        }
    }
}

void AirborneNeutralState::SwitchState(CharState s) {
    if (s == AIRBORNE) {
        std::cerr << "ATTEMPT TO SWITCH TO AIRBORNE WHILE AIRBORNENEUTRAL" << std::endl;
    } else if (s == GROUNDED) {
        // Land on stage/platform
        character_->NullVelocityX();
        character_->SetActionState(new LandingLagState(character_, 2));
        return;
    }
}
