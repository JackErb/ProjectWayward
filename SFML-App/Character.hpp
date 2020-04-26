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
#include "StageEntity.hpp"
#include "PhysicsEngine.hpp"

class CharacterState;

class Character : public Entity {
public:
    typedef enum State {
        AIRBORNE, GROUNDED
    } State;
    
public:
    Character(int id, sf::Vector2f position);
    
    void ProcessInput(const PlayerInput &input);
    void Tick();
    void HandleCollision(const Entity &entity, sf::Vector2f pv);
    
    EntityType Type() const {
        return CHARACTER;
    }
    
    void ApplyGravity() {
        velocity.y += gravity;
        velocity.y = fmin(maxFallSpeed, velocity.y);
    }
    
    void ApplyVelocity() {
        Transform(velocity);
    }
    
    void SetState(State s) {
        state_ = s;
    }
    
    /* Methods involving state, state change, or action */
    void Jumpsquat();
    void Jump();
        
public:
    StageEntity *stage_;
    
private:
    void setActionState(CharacterState *s) {
        cleanupState_ = actionState_;
        actionState_ = s;
    }
    
private:
    State state_;
    CharacterState *actionState_;
    CharacterState *cleanupState_ = nullptr;
    unsigned int jumps_;
    
    float gravity = 2.f;
    float maxFallSpeed = 30.f;
    
    friend class CharacterState;
    friend class NeutralState;
};

#endif /* Character_hpp */
