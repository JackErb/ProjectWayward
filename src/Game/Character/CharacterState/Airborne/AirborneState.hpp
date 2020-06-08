//
//  AirborneState.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/27/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef AirborneState_hpp
#define AirborneState_hpp

#include "../CharacterState.hpp"

class AirborneState : public CharacterState {
public:
    AirborneState(Character *ch) : CharacterState(ch) {}
    CharState GetState() const override final {
        return AIRBORNE;
    }
};

#endif /* AirborneState_hpp */
