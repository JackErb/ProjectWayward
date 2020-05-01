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
#include "SpriteLoader.hpp"

using std::string;

class CharacterState;

class Character : public Entity {
public:
    typedef enum JumpType {
        LEFT, UP, RIGHT,
        DJUMP
    } JumpType;
    
    typedef struct CharacterAttributes {
        float GroundAccel = 2.5f;
        float MaxGroundSpeed = 25.f;
        float GroundFriction = 0.85f;
           
        float AirAccel = 3.0f;
        float MaxAirSpeed = 30.5f;
        float AirFriction = 0.9f;
           
        float Gravity = 3.4f;
        float MaxFallSpeed = 40.f;
        float FastFallSpeed = 70.f;
        
        float DoubleJump = -65.f;
        float WallJump = -60.f;
        float FullhopJump = -63.f;
        float ShorthopJump = -50.f;
        
        float AirdodgeVelocity = 60.f;
    } Attributes;
    
    typedef enum CState {
        GROUNDED, AIRBORNE
    } CState;
    
    typedef struct GroundedData {
        const StageEntity *stage;
    } GroundedData;
    
    typedef struct AirborneData {
        int jumps;
        bool airdodge;
        bool walljump;
        bool fastfall;
    } AirborneData;
    
public:
    Character(int id, sf::Vector2f position);
    ~Character();
    
    /* Setup */
    void SetAnimMap(AnimMap anims) {
        anims_ = anims;
    }
    
    
    /* Get Methods */
    EntityType Type() const override { return CHARACTER; }
    sf::Vector2f Velocity() const { return velocity_; }
    int Direction() const override { return direction_; }
    sf::Sprite* GetSprite(string state, int frame) { return anims_[state][frame]; }
    
    
    /* Game Processing */
    void ProcessInput(const PlayerInput &input);
    void Tick() override;
    void HandleCollision(const Entity &entity, sf::Vector2f pv) override;
    

    
    
    /* Methods involving action, and should only be called by the current actionState_
     * ALL OF THESE METHODS NEED TO BE ABLE TO ROLL BACK */
    void SetActionState(CharacterState *s);
    void Jump(JumpType type, bool fullhop);
    void FastFall() { velocity_.y = attr.FastFallSpeed; airborneData.fastfall = true; }
    void NullVelocityY() { velocity_.y = 0; }
    void NullVelocityX() { velocity_.x = 0; }
    void Dash(float m);
    void Vector(float angle);
    void ApplyGravity(float m = 1.f) {
        velocity_.y += m * attr.Gravity;
        velocity_.y = fmin(attr.MaxFallSpeed, velocity_.y);
    }
    void ApplyVelocity() { Transform(velocity_); }
    void ApplyFriction();
    void FallthroughPlatform();
    void WallJump(int dir);
    void Airdodge();
    void UpB() { velocity_.y = -80.f; }
    void Turnaround() { direction_ *= -1; }
    void SetDirection(int dir) { direction_ = dir; }
    
private:
    void initAirborneData() {
        airborneData.jumps = 1;
        airborneData.walljump = true;
        airborneData.airdodge = true;
        airborneData.fastfall = false;
    }
    
public:
    // The input for this frame
    const PlayerInput* input_;
    
    union {
        GroundedData groundedData;
        AirborneData airborneData;
    };
    
private:
    CharacterState *actionState_ = nullptr;
    CharacterState *cleanupState_ = nullptr;
    unsigned int jumps_;
    
    const Entity *fallthrough_ = nullptr;
    int ftCount_;
    
    sf::Vector2f velocity_ = {0.f, 0.f};
    
    const Attributes attr;
    
    AnimMap anims_;
    
    int direction_;
};

#endif /* Character_hpp */
