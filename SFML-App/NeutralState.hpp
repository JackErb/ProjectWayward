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
    NeutralState(Character *ch) : GroundedState(ch) {
        ch->NullVelocityY();
        frame_ = 0;
    }
    ~NeutralState() {}
    
    void HandleCollision(const Entity &entity, sf::Vector2f pv) override ;
    void ProcessInput(const PlayerInput &input) override;
    void Tick() override;
    void SwitchState(Character::CState state) override;
};

#endif /* NeutralState_hpp */
