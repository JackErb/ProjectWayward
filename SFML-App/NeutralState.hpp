//
//  NeutralState.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/26/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef NeutralState_hpp
#define NeutralState_hpp

#include "CharacterState.hpp"

class NeutralState : public CharacterState {
public:
    NeutralState(Character *ch) : CharacterState(ch) {}
    
    void HandleCollision(const Entity &entity, sf::Vector2f pv);
    void ProcessInput(const PlayerInput &input);
    void Tick();
    
public:
    const float DEAD_ZONE = 6.f;
};

#endif /* NeutralState_hpp */
