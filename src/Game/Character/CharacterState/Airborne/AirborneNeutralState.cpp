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
    if (input.IsPressed(JUMP) && character->Jumps() > 0) {
        character->Jump(DJUMP, true);
    }
    
    if (input.IsPressed(SHIELD) && character->HasAirdodge()) {
        character->SetActionState(new AirdodgeState(character, input.stick.angle()));
        return;
    }
    
    if (input.IsPressed(ATTACK)) {
        if (input.stick.inDirection(DOWN_T)) {
            character->SetActionState(new AirborneScriptState(character, "DAIR"));
        } else if (input.stick.inDirection(LEFT_T) || input.stick.inDirection(RIGHT_T)) {
            character->SetActionState(new AirborneScriptState(character, "FAIR"));
        } else if (input.stick.inDirection(UP_T)) {
            character->SetActionState(new AirborneScriptState(character, "UAIR"));
        } else {
            character->SetActionState(new AirborneScriptState(character, "NAIR"));
        }
        return;
    }
    
    if (input.IsPressed(SPECIAL)) {
        character->SetActionState(new AirborneScriptState(character, "FSPECIAL"));
        return;
    }
    
     character->Vector();
}

void AirborneNeutralState::Tick() {
    character->ApplyGravity();
    character->ApplyVelocity();
}

void AirborneNeutralState::HandleCollision(const Entity &entity, VectorV pv) {
    if (entity.Type() == STAGE) {
        if (pv.x.n != 0 && pv.y.n == 0) {
            if (pv.x < 0 && character->input->stick.inDirection(Direction::RIGHT_T)
                && character->input->IsPressed(JUMP)) {
                character->WallJump(-1);
                character->SetDirection(-1);
            } else if (pv.x > 0 && character->input->stick.inDirection(Direction::LEFT_T)
                       && character->input->IsPressed(JUMP)) {
                character->WallJump(1);
                character->SetDirection(1);
            }
        }
    }
}

void AirborneNeutralState::SwitchState(CharState s) {
    if (s == AIRBORNE) {
        std::cerr << "ATTEMPT TO SWITCH TO AIRBORNE WHILE AIRBORNENEUTRAL" << std::endl;
    } else if (s == GROUNDED) {
        // Land on stage/platform
        character->NullVelocityX();
        character->SetActionState(new LandingLagState(character, 2));
        return;
    }
}
