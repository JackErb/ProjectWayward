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
        data.frame = 7;
        Turnaround();
    }
    
    TurnaroundState(Character *ch, bool rb) : GroundedState(ch) {}
    
    void ProcessInput(const PlayerInput &input) override;
    void Tick() override;
    void HandleCollision(const Entity &e, VectorV pv) override;
    void SwitchState(CharState state) override;
    
    CharStateType GetStateType() const override { return CharStateType::Turnaround; }
    
private:
    void Turnaround();
};

#endif /* TurnaroundState_hpp */
