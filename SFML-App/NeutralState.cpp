//
//  NeutralState.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/26/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "NeutralState.hpp"

#define PI 3.14159

void NeutralState::ProcessInput(const PlayerInput &input) {
    float hyp = input.stick.hyp();
    float angle = input.stick.angle();
    if (hyp > DEAD_ZONE) {
        float speed = 16.5f;
        float mspeed = (hyp > 60.f ? 1.f : hyp / 60.f) * speed;
        
        sf::Vector2f vec = {cos(angle) * mspeed, 0};
        character_->Transform(vec);
    }
    
    switch (character_->state_) {
        case Character::GROUNDED:
            if (input.IsPressed(3)) {
                character_->Jumpsquat();
            }
            break;
        case Character::AIRBORNE:
            if (input.IsPressed(3)) {
                character_->Jump();
            }
            
            if (angle >= -3.f / 4.f * PI && angle <= 1.f / 4.f * PI && hyp > 40.f) {
                // Fast fall ?
            }
            break;
    }
}

void NeutralState::Tick() {
    switch (character_->state_) {
        case Character::GROUNDED:
            break;
        case Character::AIRBORNE:
            character_->ApplyGravity();
            break;
    }
    
    character_->ApplyVelocity();
}

void NeutralState::HandleCollision(const Entity &entity, sf::Vector2f pv) {
    if (entity.Type() == STAGE) {
        if (pv.x == 0 && pv.y < 0 && character_->velocity.y > 0) {
            character_->velocity.y = 0;
            character_->SetState(Character::GROUNDED);
        }
        
        character_->Transform(pv);
    } else if (entity.Type() == PLATFORM) {
        if (pv.x == 0 && pv.y < 0 && character_->velocity.y > 0) {
            character_->velocity.y = 0;
            character_->SetState(Character::GROUNDED);
            
            character_->Transform(pv);
        }
    }
}
