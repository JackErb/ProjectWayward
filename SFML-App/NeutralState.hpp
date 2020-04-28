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
    NeutralState(Character *ch, Entity* const s) : GroundedState(ch, s), fastfall(false) {
        ch->NullVelocityY();
    }
    ~NeutralState() {}
    
    void HandleCollision(const Entity &entity, sf::Vector2f pv) override ;
    void ProcessInput(const PlayerInput &input) override;
    void Tick() override;
    void SwitchState(State state) override;
    
public:
    const float DEAD_ZONE = 6.f;
    bool fastfall;
};

#endif /* NeutralState_hpp */
