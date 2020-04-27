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
    JumpsquatState(Character *ch) : GroundedState(ch), type(Character::UP) {
        stateFrame_ = 1;
    }
    ~JumpsquatState() {}
    
    void HandleCollision(const Entity &entity, sf::Vector2f pv);
    void ProcessInput(const PlayerInput &input);
    void Tick();
    void SwitchState(State state) {}
    
private:
    int stateFrame_;
    Character::JumpType type;
    bool jumpHeld = true;
};

#endif /* JumpsquatState_hpp */
