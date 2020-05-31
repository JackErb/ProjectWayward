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
        data.frame_ = 0;
    }
    ~NeutralState() {}
    
    void ProcessInput(const PlayerInput &input) override;
    void Tick() override;
    
    void HandleCollision(const Entity &entity, sf::Vector2f pv) override;
    
    void SwitchState(CharState state) override;
    CharStateType GetStateType() const override { return Neutral; }
    
private:
    void NullVelocity();
};

#endif /* NeutralState_hpp */
