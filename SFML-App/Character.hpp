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
#include <math.h>

#include "Entity.hpp"
#include "PlayerInput.hpp"
#include "StageEntity.hpp"

class CharacterState;

class Character : public Entity {
public:
    typedef enum JumpType {
        LEFT, UP, RIGHT,
        DJUMP
    } JumpType;
    
public:
    Character(int id, sf::Vector2f position);
    
    void ProcessInput(const PlayerInput &input);
    void Tick();
    void HandleCollision(const Entity &entity, sf::Vector2f pv);
    
    void SetActionState(CharacterState *s);
    EntityType Type() const { return CHARACTER; }
    sf::Vector2f Velocity() const { return velocity_; }
    
    /* Methods involving action, and should only be called by the current actionState_ */
    void Jump(JumpType type, bool fullhop);
    void FastFall() { velocity_.y = FastFallSpeed; }
    void NullVelocityY() { velocity_.y = 0; }
    void Dash(float m);
    void Vector(float angle);
    void ApplyGravity() {
        velocity_.y += Gravity;
        velocity_.y = fmin(MaxFallSpeed, velocity_.y);
    }
    void ApplyVelocity() { Transform(velocity_); }
    void ApplyFriction();
    
private:
    CharacterState *actionState_;
    CharacterState *cleanupState_ = nullptr;
    unsigned int jumps_;
    
    sf::Vector2f velocity_ = {0.f, 0.f};
    
    const float GroundAccel = 2.5f;
    const float MaxGroundSpeed = 25.f;
    const float GroundFriction = 0.95f;
    
    const float AirAccel = 3.0f;
    const float MaxAirSpeed = 30.5f;
    const float AirFriction = 0.95f;
    
    const float Gravity = 3.4f;
    const float MaxFallSpeed = 40.f;
    const float FastFallSpeed = 60.f;
    
    const float JumpSpeed = -50.f;
};

#endif /* Character_hpp */
