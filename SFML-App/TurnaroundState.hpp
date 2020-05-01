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
public:
    TurnaroundState(Character *ch) : GroundedState(ch) {
        character_->NullVelocityX();
        character_->Turnaround();
        frame_ = 7;
    }
    
    void ProcessInput(const PlayerInput &input) override;
    void Tick() override;
    void HandleCollision(const Entity &e, sf::Vector2f pv) override;
    void SwitchState(Character::CState state) override;
    
    CharacterStateType GetStateType() const override { return Turnaround; }
};

#endif /* TurnaroundState_hpp */
