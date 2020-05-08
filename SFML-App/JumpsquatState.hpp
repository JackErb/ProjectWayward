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

class JumpsquatState : public GroundedState {
public:
    JumpsquatState(Character *ch) : GroundedState(ch) {
        data.type_ = 0;
        data.frame_ = 0;
        data.b1 = true;
    }
    ~JumpsquatState() {}
    
    void HandleCollision(const Entity &entity, sf::Vector2f pv) override;
    void ProcessInput(const PlayerInput &input) override;
    void Tick() override;
    void SwitchState(CharState state) override {}
    
    CharStateType GetStateType() const override { return Jumpsquat; }
    
private:
    bool jumpHeld = true;
};

#endif /* JumpsquatState_hpp */
