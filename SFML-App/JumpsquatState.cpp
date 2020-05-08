//
//  JumpsquatState.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/26/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "JumpsquatState.hpp"
#include "AirborneNeutralState.hpp"
#include "Character.hpp"

void JumpsquatState::ProcessInput(const PlayerInput &input) {
    if (input.stick.hyp() > 40.f) {
        float a = input.stick.angle();
        if (input.stick.inDirection(LEFT) && data.type_ == JUP) {
            data.type_ = JLEFT;
        } else if (input.stick.inDirection(RIGHT) && data.type_ == JUP) {
            data.type_ = JRIGHT;
        }
    }
    
    if (input.IsReleased(3)) {
        data.b1 = false;
    }
}

void JumpsquatState::Tick() {
    if (data.frame_ >= 3) {
        character_->SetActionState(new AirborneNeutralState(character_));
        character_->Jump(JumpType(data.type_), data.b1);
        return;
    }
    
    data.frame_++;
}

void JumpsquatState::HandleCollision(const Entity &e1, sf::Vector2f pv) {
    // TODO: _
}
