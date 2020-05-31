//
//  GroundedState.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/27/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef GroundedState_hpp
#define GroundedState_hpp

#include "CharacterState.hpp"

class GroundedState : public CharacterState {
public:
    GroundedState(Character *ch) : CharacterState(ch) {}

    CharState GetState() const override final {
        return GROUNDED;
    }
};

#endif /* GroundedState_hpp */
