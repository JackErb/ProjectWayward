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
#include "MathHelper.hpp"

class Character;

class CharacterState {
public:
    CharacterState(Character *ch) : character_(ch) {}
    virtual ~CharacterState() {}
    
    virtual void HandleCollision(const Entity &entity, sf::Vector2f pv) = 0;
    virtual void ProcessInput(const PlayerInput &input) = 0;
    virtual void Tick() = 0;
    virtual void SwitchState(Character::CState state) = 0;
    virtual Character::CState GetState() = 0;
    
protected:
    Character *character_;
    int frame_;
};

#endif /* CharacterState_hpp */
