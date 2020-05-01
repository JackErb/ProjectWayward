//
//  AirborneMoveState.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/28/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef AirborneMoveState_hpp
#define AirborneMoveState_hpp

#include "AirborneState.hpp"

class AirborneMoveState : public AirborneState {
public:
    AirborneMoveState(Character *ch) : AirborneState(ch) {}
    
    void ProcessInput(const PlayerInput &input) override;
    void Tick() override;
    void HandleCollision(const Entity &e, sf::Vector2f pv) override;
    void SwitchState(Character::CState state) override;
    
    CharacterStateType GetStateType() const override { return AirborneMove; }
};

#endif /* AirborneMoveState_hpp */
