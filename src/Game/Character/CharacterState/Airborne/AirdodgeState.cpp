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
    character->Airdodge();
}

void AirdodgeState::ProcessInput(const PlayerInput &input) {
    
}

void AirdodgeState::Tick() {
    data.frame++;
    character->ApplyVelocity();
    character->ApplyFriction();
    if (PlayerInput::InDir(data.dirInfluence, UP)) {
        character->ApplyGravity(fpoat(0,85));
    } else if (!PlayerInput::InDir(data.dirInfluence, DOWN)){
        character->ApplyGravity(fpoat(0,2));
    }
    
    if (data.frame == 25) {
        character->SetActionState(new AirborneNeutralState(character));
        return;
    }
}

void AirdodgeState::HandleCollision(const Entity &e, VectorV pv) {
    if (e.Type() == Ent_Stage) {
        // Apply the push vector to prevent overlap
        character->Transform(pv);
        
        if (pv.x.n == 0 && pv.y < 0 && character->Velocity().y > 0) {
            // Land on the stage
            character->NullVelocityY();
            character->SetStage(dynamic_cast<const StageEntity*>(&e));
            character->SetActionState(new LandingLagState(character, 5));
            return;
        } else if (pv.x.n != 0 && pv.y.n == 0) {
            if (pv.x < 0 && character->input->stick.inDirection(Direction::LEFT_T)) {
                character->WallJump(-1);
            } else if (pv.x > 0 && character->input->stick.inDirection(Direction::RIGHT_T)) {
                character->WallJump(1);
            }
        }
    } else if (e.Type() == Ent_Platform) {
        if (pv.x.n == 0 && pv.y < 0) {
            // The character collided with the platform. Check if the character
            // is above the platform and falling down
            fpoat vy = character->Velocity().y;
            Rectangle b = character->BoundingBox();
			fpoat py = b.y + b.h;
            if (!vy.sign && py - vy - fpoat(2, 0) < e.Position().y) {
                // Land on the platform
                character->NullVelocityY();
                // Apply the push vector to prevent overlap
                character->Transform(pv);
                
                character->SetStage(dynamic_cast<const StageEntity*>(&e));
                character->SetActionState(new LandingLagState(character, 5));
                return;
            }
        }
    }
}

void AirdodgeState::SwitchState(CharState state) {
    if (state == State_Airborne) {
        std::cerr << "ATTEMPT TO SWITCH TO AIRBORNE WHILE AIRDODGESTATE" << std::endl;
    } else if (state == State_Grounded) {
        // Land on stage/platform
        character->SetActionState(new LandingLagState(character, 5));
        return;
    }
}
