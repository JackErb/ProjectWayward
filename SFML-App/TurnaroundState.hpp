//
//  TurnaroundState.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef TurnaroundState_hpp
#define TurnaroundState_hpp

#include "GroundedState.hpp"

class TurnaroundState : public GroundedState {
    TurnaroundState(Character *ch) : GroundedState(ch) {}
    
    void ProcessInput(const PlayerInput &input);
    void Tick();
    void HandleCollision(const Entity &e, sf::Vector2f pv);
    void SwitchState(Character::CState state);
};

#endif /* TurnaroundState_hpp */
