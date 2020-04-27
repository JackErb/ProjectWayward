//
//  NeutralState.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/26/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef NeutralState_hpp
#define NeutralState_hpp

#include "GroundedState.hpp"

class NeutralState : public GroundedState {
public:
    NeutralState(Character *ch) : GroundedState(ch), fastfall(false) {
        ch->NullVelocityY();
    }
    ~NeutralState() {}
    
    void HandleCollision(const Entity &entity, sf::Vector2f pv);
    void ProcessInput(const PlayerInput &input);
    void Tick();
    void SwitchState(State state);
    
public:
    const float DEAD_ZONE = 6.f;
    bool fastfall;
};

#endif /* NeutralState_hpp */
