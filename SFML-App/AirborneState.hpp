//
//  AirborneState.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/27/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef AirborneState_hpp
#define AirborneState_hpp

#include "CharacterState.hpp"

class AirborneState : public CharacterState {
public:
    AirborneState(Character *ch) : CharacterState(ch) {}
    
    virtual void HandleCollision(const Entity &entity, sf::Vector2f pv) override = 0;
    virtual void ProcessInput(const PlayerInput &input) override = 0;
    virtual void Tick() override = 0;
    virtual void SwitchState(State state) override = 0;
    State GetState() override final {
        return State::AIRBORNE;
    }
};

#endif /* AirborneState_hpp */
