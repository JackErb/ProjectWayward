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
    GroundedState(Character *ch, Entity* const s) : CharacterState(ch), stage_(s) {}
    
    virtual void HandleCollision(const Entity &entity, sf::Vector2f pv) override = 0;
    virtual void ProcessInput(const PlayerInput &input) override = 0;
    virtual void Tick() override = 0;
    virtual void SwitchState(State state) override = 0;
    State GetState() override final {
        return State::GROUNDED;
    }
    Entity* GetStage() {
        return stage_;
    }
    
private:
    // The stage or platform the player is standing on
    Entity* const stage_;
};

#endif /* GroundedState_hpp */
