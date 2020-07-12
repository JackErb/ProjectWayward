//
//  JumpsquatState.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/26/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "JumpsquatState.hpp"
#include "../Airborne/AirborneNeutralState.hpp"
#include "../../Character.hpp"

void JumpsquatState::ProcessInput(const PlayerInput &input) {
    if (input.stick.hyp() > 0.3f) {
        if (input.stick.inDirection(LEFT) && data.type == JUP) {
            data.type = JLEFT;
        } else if (input.stick.inDirection(RIGHT) && data.type == JUP) {
            data.type = JRIGHT;
        }
    }
    
    if (input.IsReleased(JUMP)) {
        data.b1 = false;
    }
}

void JumpsquatState::Tick() {
    if (data.frame >= 3) {
        character->SetActionState(new AirborneNeutralState(character));
        character->Jump(JumpType(data.type), data.b1);
        return;
    }
    
    data.frame++;
}

void JumpsquatState::HandleCollision(const Entity &e1, VectorV pv) {
    // TODO: _
}

void JumpsquatState::SwitchState(CharState state) {
    switch (state) {
        case AIRBORNE:
            character->SetActionState(new AirborneNeutralState(character));
            return;
        case GROUNDED:
            std::cerr << "ERROR SWITCH STATE JUMPSQUAT" << std::endl;
            return;
    }
}
