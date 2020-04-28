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
    JumpsquatState(Character *ch, Entity* const s) : GroundedState(ch, s), type(Character::UP) {
        stateFrame_ = 1;
    }
    ~JumpsquatState() {}
    
    void HandleCollision(const Entity &entity, sf::Vector2f pv) override;
    void ProcessInput(const PlayerInput &input) override;
    void Tick() override;
    void SwitchState(State state) override {}
    
private:
    int stateFrame_;
    Character::JumpType type;
    bool jumpHeld = true;
};

#endif /* JumpsquatState_hpp */
