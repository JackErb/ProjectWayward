//
//  Character.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/25/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef Character_hpp
#define Character_hpp

#include <iostream>
#include <SFML/Graphics.hpp>

#include "Entity.hpp"
#include "PlayerInput.hpp"

typedef enum CharacterState {
    GROUNDED, JUMPSQUAT, AIRBORNE
} CharacterState;

class Character : public Entity {
public:
    Character(int id, sf::Vector2f position) : Entity(id, position) {}
    
    void ProcessInput(const PlayerInput &input);
    
    void Tick();
    void HandleCollision(const Entity &entity, sf::Vector2f pv);
    
    CharacterState State() { return state_; }
    void SetState(CharacterState s) {
        if (state_ == s
            || (state_ == JUMPSQUAT && s == GROUNDED)) return;
        
        // Verify that new state is valid
        std::cout << "State " << s << std::endl;
        
        // Set state
        state_ = s;
        stateFrame_ = 0;
        
        switch (state_) {
            case GROUNDED:
                break;
            case AIRBORNE:
                break;
            case JUMPSQUAT:
                break;
        }
    }
    
private:
    CharacterState state_;
    int stateFrame_;
    
    unsigned int jumps;
    
    const float DEAD_ZONE = 6.f;
};

#endif /* Character_hpp */
