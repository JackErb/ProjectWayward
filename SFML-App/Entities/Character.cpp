//
//  Character.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/25/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "Character.hpp"
#include "PlayerInput.hpp"

#include <algorithm>
#include <math.h>
#include <SFML/Graphics.hpp>

void Character::Tick() {
    switch (state_) {
        case GROUNDED:
            // Check if airborne
            break;
        case AIRBORNE:
            velocity.y += gravity;
            velocity.y = fmin(velocity.y, maxFallSpeed);
            Transform(velocity);
            break;
        case JUMPSQUAT:
            if (stateFrame_ >= 3) {
                SetState(AIRBORNE);
                jumps = 1;
                velocity.y = -30;
            }
            break;
    }
    
    stateFrame_++;
}

void Character::ProcessInput(const PlayerInput &input) {
    if (state_ == JUMPSQUAT) return;
    
    float hyp = sqrt(pow(input.stick.xAxis, 2) + pow(input.stick.yAxis, 2));
    if (hyp > DEAD_ZONE) {
        float angle = atan2(input.stick.yAxis, input.stick.xAxis);
                
        float speed = 10.f;
        float mspeed = (hyp > 60.f ? 1.f : hyp / 60.f) * speed;
        
        sf::Vector2f vec = {cos(angle) * mspeed, 0};
        Transform(vec);
    } else {
        // Directional input is within dead zone of stick; ignore it
    }
        
    if (input.IsPressed(3)) {
        switch (state_) {
            case GROUNDED:
                SetState(JUMPSQUAT);
                break;
            case AIRBORNE:
                if (jumps >= 1){
                    velocity.y = -40;
                    jumps--;
                }
                break;
            default: break;
        }
    }
}

void Character::HandleCollision(const Entity &entity, sf::Vector2f pv) {
    if (entity.isStatic) {
        if (pv.x == 0 && pv.y < 0) {
            SetState(GROUNDED);
        }
    }
}
