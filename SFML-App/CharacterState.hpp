//
//  CharacterState.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/26/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef CharacterState_hpp
#define CharacterState_hpp

#include "Character.hpp"

class Character;

class CharacterState {
public:
    CharacterState(Character *ch) : character_(ch) {}
    
    virtual void HandleCollision(const Entity &entity, sf::Vector2f pv) = 0;
    virtual void ProcessInput(const PlayerInput &input) = 0;
    virtual void Tick() = 0;
    
public:
    Character *character_;
};

#endif /* CharacterState_hpp */
