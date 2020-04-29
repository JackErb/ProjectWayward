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

class StageEntity;

class GroundedState : public CharacterState {
public:
    GroundedState(Character *ch) : CharacterState(ch) {}
    
    virtual void HandleCollision(const Entity &entity, sf::Vector2f pv) override = 0;
    virtual void ProcessInput(const PlayerInput &input) override = 0;
    virtual void Tick() override = 0;
    virtual void SwitchState(Character::CState state) override = 0;
    Character::CState GetState() override final {
        return Character::GROUNDED;
    }
};

#endif /* GroundedState_hpp */
