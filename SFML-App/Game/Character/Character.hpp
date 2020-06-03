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
#include <list>

#include "Entity.hpp"
#include "PlayerInput.hpp"
#include "StageEntity.hpp"
#include "SpriteLoader.hpp"
#include "CharacterState.hpp"
#include "CharacterProperties.hpp"
#include "MoveLoader.hpp"

using std::string;
using std::list;

class MslInterpreter;

class Character : public Entity {
public:
    typedef struct CharacterAttributes {
        float GroundAccel = 2.5f;
        float MaxGroundSpeed = 30.f;
        float GroundFriction = 0.87f;
           
        float AirAccel = 3.5f;
        float MaxAirSpeed = 38.f;
        float AirFriction = 0.91f;
           
        float Gravity = 3.4f;
        float MaxFallSpeed = 40.f;
        float FastFallSpeed = 70.f;
        
        float DoubleJump = -65.f;
        float WallJump = -60.f;
        float FullhopJump = -63.f;
        float ShorthopJump = -50.f;
        
        float AirdodgeVelocity = 60.f;
    } Attributes;
    
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
    sf::Vector2f Velocity() const { return data.velocity_; }
    int Direction() const override { return data.direction_; }
    sf::Sprite* GetSprite(string state, int frame) { return anims_[state][frame]; }
    const StageEntity *Stage() const { return data.groundedData.stage; }
    int Jumps() const { return data.airborneData.jumps; }
    bool IsFastFalling() const  { return data.airborneData.fastfall; }
    bool HasAirdodge() const { return data.airborneData.airdodge; }
    
    
    /* Game Processing */
    void ProcessInput(const PlayerInput &input);
    void Tick() override;
    void HandleCollision(const Entity &entity, sf::Vector2f pv) override;
    
    void RollbackTick() override;
    void Rollback() override;
    
    
    /* Methods involving action, and should only be called by the current actionState_
     * ALL OF THESE METHODS NEED TO BE ABLE TO ROLL BACK */
    void SetActionState(CharacterState *s);
    void Jump(JumpType type, bool fullhop);
    void FastFall() { data.velocity_.y = attr.FastFallSpeed; data.airborneData.fastfall = true; }
    void NullVelocityY() { data.velocity_.y = 0; }
    void NullVelocityX() { data.velocity_.x = 0; }
    void Dash(float m);
    void Vector(float v = 1.f);
    void ApplyGravity(float m = 1.f) {
        if (data.airborneData.fastfall) {
            FastFall();
            return;
        }
        data.velocity_.y += m * attr.Gravity;
        data.velocity_.y = fmin(attr.MaxFallSpeed, data.velocity_.y);
    }
    void ApplyVelocity() { Transform(data.velocity_); }
    void ApplyFriction();
    void FallthroughPlatform();
    void WallJump(int dir);
    void Airdodge();
    void UpB() { data.velocity_.y = -80.f; }
    void Turnaround() { data.direction_ *= -1; }
    void SetDirection(int dir) { data.direction_ = dir; }
    void SetStage(const StageEntity *s) { data.groundedData.stage = s; }
    void Airborne() { initAirborneData(); }
    
    
    void IncRot(float n) { Sprite()->rotate(n); }
    
private:
    void initAirborneData() {
        data.airborneData.jumps = 1;
        data.airborneData.walljump = true;
        data.airborneData.airdodge = true;
        data.airborneData.fastfall = false;
    }
    
public:
    // The input for this frame
    const PlayerInput* input_;
    MslInterpreter *mslIntp;
    
    bool fill = false;
    
private:
    typedef struct CharacterData {
        CharStateType actionStateT_;
        CharacterState::GameData actionStateData_;
        
        sf::Vector2f velocity_ = {0.f, 0.f};
        int direction_ = 1;
        int fallthrough_ = -1;
        int ftCount_ = 0;
        
        union {
            GroundedData groundedData;
            AirborneData airborneData;
        };
    } GameData;
    
    // Pointer to the current game data state.
    GameData data;
    GameData rollback_;
    
    CharacterState *actionState_;
    
    const Attributes attr;
    AnimMap anims_;
};

#endif /* Character_hpp */
