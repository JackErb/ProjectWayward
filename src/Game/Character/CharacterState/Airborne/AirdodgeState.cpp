//
//  AirdodgeState.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/28/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "AirdodgeState.hpp"
#include "AirborneNeutralState.hpp"
#include "../Grounded/LandingLagState.hpp"
#include "../../Character.hpp"

void AirdodgeState::Airdodge() {
    character_->Airdodge();
}

void AirdodgeState::ProcessInput(const PlayerInput &input) {
    
}

void AirdodgeState::Tick() {
    data.frame_++;
    character_->ApplyVelocity();
    character_->ApplyFriction();
    if (PlayerInput::InDir(data.dirInfluence_, UP)) {
        character_->ApplyGravity(0.85f);
    } else if (!PlayerInput::InDir(data.dirInfluence_, DOWN)){
        character_->ApplyGravity(0.2f);
    }
    
    if (data.frame_ == 25) {
        character_->SetActionState(new AirborneNeutralState(character_));
        return;
    }
}

void AirdodgeState::HandleCollision(const Entity &e, VectorV pv) {
    if (e.Type() == STAGE) {
        // Apply the push vector to prevent overlap
        character_->Transform(pv);
        
        if (pv.x.n == 0 && pv.y < 0 && character_->Velocity().y > 0) {
            // Land on the stage
            character_->NullVelocityY();
            character_->SetStage(dynamic_cast<const StageEntity*>(&e));
            character_->SetActionState(new LandingLagState(character_, 5));
            return;
        } else if (pv.x.n != 0 && pv.y.n == 0) {
            if (pv.x < 0 && character_->input_->stick.inDirection(Direction::LEFT_T)) {
                character_->WallJump(-1);
            } else if (pv.x > 0 && character_->input_->stick.inDirection(Direction::RIGHT_T)) {
                character_->WallJump(1);
            }
        }
    } else if (e.Type() == PLATFORM) {
        if (pv.x.n == 0 && pv.y < 0) {
            // The character collided with the platform. Check if the character
            // is above the platform and falling down
            fpoat vy = character_->Velocity().y;
            Rectangle b = character_->BoundingBox();
			fpoat py = b.y + b.h;
            if (!vy.sign && py - vy - fpoat(2, 0) < e.Position().y) {
                // Land on the platform
                character_->NullVelocityY();
                // Apply the push vector to prevent overlap
                character_->Transform(pv);
                
                character_->SetStage(dynamic_cast<const StageEntity*>(&e));
                character_->SetActionState(new LandingLagState(character_, 5));
                return;
            }
        }
    }
}

void AirdodgeState::SwitchState(CharState state) {
    if (state == AIRBORNE) {
        std::cerr << "ATTEMPT TO SWITCH TO AIRBORNE WHILE AIRDODGESTATE" << std::endl;
    } else if (state == GROUNDED) {
        // Land on stage/platform
        character_->SetActionState(new LandingLagState(character_, 5));
        return;
    }
}
