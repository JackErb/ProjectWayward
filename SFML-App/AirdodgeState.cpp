//
//  AirdodgeState.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/28/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "AirdodgeState.hpp"
#include "AirborneNeutralState.hpp"
#include "LandingLagState.hpp"

void AirdodgeState::ProcessInput(const PlayerInput &input) {
    
}

void AirdodgeState::Tick() {
    frame_++;
    character_->ApplyVelocity();
    character_->ApplyFriction();
    if (PlayerInput::InDir(angle_, UP)) {
        character_->ApplyGravity(0.85f);
    } else if (!PlayerInput::InDir(angle_, DOWN)){
        character_->ApplyGravity(0.2f);
    }
    
    if (frame_ == 25) {
        character_->SetActionState(new AirborneNeutralState(character_));
        return;
    }
}

void AirdodgeState::HandleCollision(const Entity &e, sf::Vector2f pv) {
    if (e.Type() == STAGE) {
        // Apply the push vector to prevent overlap
        character_->Transform(pv);
        
        if (pv.x == 0 && pv.y < 0 && character_->Velocity().y > 0) {
            // Land on the stage
            character_->NullVelocityY();
            character_->groundedData.stage = dynamic_cast<const StageEntity*>(&e);
            character_->SetActionState(new LandingLagState(character_, 5));
            return;
        } else if (abs(pv.x) > 0 && pv.y == 0) {
            if (pv.x < 0 && character_->input_->stick.inDirection(Direction::LEFT_T)) {
                character_->WallJump(-1);
            } else if (pv.x > 0 && character_->input_->stick.inDirection(Direction::RIGHT_T)) {
                character_->WallJump(1);
            }
        }
    } else if (e.Type() == PLATFORM) {
        if (pv.x == 0 && pv.y < 0) {
            // The character collided with the platform. Check if the character
            // is above the platform and falling down
            float vy = character_->Velocity().y;
            Rectangle b = character_->BoundingBox();
            float py = b.y + b.h;
            if (vy > 0 && py - vy - 2.f < e.Position().y) {
                // Land on the platform
                character_->NullVelocityY();
                // Apply the push vector to prevent overlap
                character_->Transform(pv);
                
                character_->groundedData.stage = dynamic_cast<const StageEntity*>(&e);
                character_->SetActionState(new LandingLagState(character_, 5));
                return;
            }
        }
    }
}

void AirdodgeState::SwitchState(Character::CState state) {
    if (state == Character::AIRBORNE) {
        std::cerr << "ATTEMPT TO SWITCH TO AIRBORNE WHILE AIRDODGESTATE" << std::endl;
    } else if (state == Character::GROUNDED) {
        // Land on stage/platform
        character_->SetActionState(new LandingLagState(character_, 5));
        return;
    }
}
