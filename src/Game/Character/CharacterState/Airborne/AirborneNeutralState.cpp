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

AirborneNeutralState::AirborneNeutralState(Character* ch) : AirborneState(ch) {
    
}

void AirborneNeutralState::ProcessInput(const PlayerInput &input) {
    if (input.IsPressed(3) && character_->Jumps() > 0) {
        character_->Jump(DJUMP, true);
    }
    
    if (input.IsPressed(7) && character_->HasAirdodge()) {
        character_->SetActionState(new AirdodgeState(character_, input.stick.angle()));
        return;
    }
    
    if (input.IsPressed(1)) {
        character_->SetActionState(new AirborneScriptState(character_, "NAIR"));
        return;
    }
    
    if (input.IsPressed(0) && input.stick.inDirection(UP_T)) {
        character_->UpB();
        return;
    }
    
     character_->Vector();
}

void AirborneNeutralState::Tick() {
    character_->ApplyGravity();
    character_->ApplyVelocity();
}

void AirborneNeutralState::HandleCollision(const Entity &entity, sf::Vector2f pv) {
    if (entity.Type() == STAGE) {
        if (abs(pv.x) > 0 && pv.y == 0) {
            if (pv.x < 0 && character_->input_->stick.inDirection(Direction::RIGHT_T)
                && character_->input_->IsPressed(3)) {
                character_->WallJump(-1);
                character_->SetDirection(-1);
            } else if (pv.x > 0 && character_->input_->stick.inDirection(Direction::LEFT_T)
                       && character_->input_->IsPressed(3)) {
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
