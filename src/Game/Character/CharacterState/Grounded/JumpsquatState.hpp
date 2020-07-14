//
//  JumpsquatState.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/26/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef JumpsquatState_hpp
#define JumpsquatState_hpp

#include "GroundedState.hpp"
#include "../../CharacterProperties.hpp"

class JumpsquatState : public GroundedState {
public:
    JumpsquatState(Character *ch) : GroundedState(ch) {
        data.type = Jump_Up;
        data.frame = 0;
        data.b1 = true;
    }
    JumpsquatState(Character *ch, bool rb) : GroundedState(ch) {}
    ~JumpsquatState() {}
    
    void HandleCollision(const Entity &entity, VectorV pv) override;
    void ProcessInput(const PlayerInput &input) override;
    void Tick() override;
    void SwitchState(CharState state) override;
    
    CharStateType GetStateType() const override { return Jumpsquat; }
};

#endif /* JumpsquatState_hpp */
