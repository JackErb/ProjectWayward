//
//  AirborneState.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/27/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "AirborneNeutralState.hpp"
#include "NeutralState.hpp"
#include "LandingLagState.hpp"

void AirborneNeutralState::ProcessInput(const PlayerInput &input) {
    if (input.IsPressed(3)) {
        character_->Jump(Character::DJUMP, true);
    }
    
    float a = input.stick.angle();
    float hyp = input.stick.hyp();
    if (PlayerInput::InRange(a, -3.f / 4.f * M_PI, - 1.f / 4.f * M_PI) && hyp > 65.f) {
        character_->FastFall();
        fastfall_ = true;
    }
    
    if (hyp > PlayerInput::DEAD_ZONE) {
        float m = (input.stick.xAxis > 60.f ? 60.f : input.stick.xAxis) / 60.f;
        character_->Vector(m);
    } else {
        character_->ApplyFriction();
    }
}

void AirborneNeutralState::Tick() {
    character_->ApplyGravity();
    character_->ApplyVelocity();
}

void AirborneNeutralState::HandleCollision(const Entity &entity, sf::Vector2f pv) {
    std::cout << "COLLIDE" << std::endl;
    if (entity.Type() == STAGE) {
        // Apply the push vector to prevent overlap
        character_->Transform(pv);
        
        if (pv.x == 0 && pv.y < 0 && character_->Velocity().y > 0) {
            // Land on the stage
            character_->NullVelocityY();
            character_->SetActionState(new LandingLagState(character_, 2));
            return;
        }
    } else if (entity.Type() == PLATFORM) {
        if (pv.x == 0 && pv.y < 0) {
            // The character collided with the platform. Check if the character
            // is above the platform and falling down
            float vy = character_->Velocity().y;
            Rectangle b = character_->BoundingBox();
            float py = b.y + b.h;
            if (vy > 0 && py - vy - 2.f < entity.Position().y) {
                // Land on the platform
                character_->NullVelocityY();
                // Apply the push vector to prevent overlap
                character_->Transform(pv);
                
                character_->SetActionState(new LandingLagState(character_, 2));
                return;
            }
        }
    }
}
