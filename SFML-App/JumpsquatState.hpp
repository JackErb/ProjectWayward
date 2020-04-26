//
//  JumpsquatState.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/26/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef JumpsquatState_hpp
#define JumpsquatState_hpp

#include "CharacterState.hpp"

class JumpsquatState : public CharacterState {
public:
    JumpsquatState(Character *ch) : CharacterState(ch) {
        stateFrame_ = 0;
    }
    
    void HandleCollision(const Entity &entity, sf::Vector2f pv);
    void ProcessInput(const PlayerInput &input);
    void Tick();
    
private:
    int stateFrame_;
};

#endif /* JumpsquatState_hpp */
