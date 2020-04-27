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
    
    virtual void HandleCollision(const Entity &entity, sf::Vector2f pv) = 0;
    virtual void ProcessInput(const PlayerInput &input) = 0;
    virtual void Tick() = 0;
    virtual void SwitchState(State state) = 0;
    State GetState() final {
        return State::GROUNDED;
    }
};

#endif /* GroundedState_hpp */
