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
    if (input.stick.inDirection(DOWN_T) && hyp > 50.f) {
        character_->FastFall();
        fastfall_ = true;
    }
    
    if (hyp > PlayerInput::DEAD_ZONE) {
        float m = (abs(input.stick.xAxis) > 60.f ? 60.f : abs(input.stick.xAxis)) / 60.f;
        if (input.stick.xAxis < 0) m *= -1;
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
    if (entity.Type() == STAGE) {
        // Apply the push vector to prevent overlap
        character_->Transform(pv);
        
        if (pv.x == 0 && pv.y < 0 && character_->Velocity().y > 0) {
            // Land on the stage
            character_->NullVelocityY();
            character_->SetActionState(new LandingLagState(character_,
                                            const_cast<Entity* const>(&entity), 2));
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
                
                character_->SetActionState(new LandingLagState(character_,
                                                const_cast<Entity *const>(&entity), 2));
                return;
            }
        }
    }
}
