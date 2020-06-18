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
#include <vector>
#include <set>

#include "../Entities/Entity.hpp"
#include "../PlayerInput.hpp"
#include "../Entities/StageEntity.hpp"
#include "../Loaders/SpriteLoader.hpp"
#include "CharacterState/CharacterState.hpp"
#include "CharacterProperties.hpp"
#include "../Loaders/MoveLoader.hpp"

using std::string;
using std::list;
using std::vector;

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
    sf::Sprite* GetSprite(string state, int frame) { return anims_[state][frame]; }
    const StageEntity *Stage() const { return data.groundedData.stage; }
    int Jumps() const { return data.airborneData.jumps; }
    bool IsFastFalling() const  { return data.airborneData.fastfall; }
    bool HasAirdodge() const { return data.airborneData.airdodge; }
    float Percent() const { return data.percent_; }
    
    
    /* Game Processing */
    void ProcessInput(const PlayerInput &input);
    void Tick() override;
    void HandleCollision(const Entity &entity, sf::Vector2f pv) override;
    bool HandleHit(const Entity *e, int f, const HitboxData &hd) override;
    
    void RollbackTick() override;
    void Rollback() override;
    
    /* Methods involving action, and should only be called by the current actionState_
     * ALL OF THESE METHODS NEED TO BE ABLE TO ROLL BACK */
    void SetActionState(CharacterState *s);
    void Jump(JumpType type, bool fullhop);
    void FastFall();
    void Knockback(float angle, float kb) {
        SetVelocity(cos(angle) * kb, sin(angle) * kb);
    }
    void Dash(float m);
    void Vector(float v = 1.f);
    void ApplyGravity(float m = 1.f);
    void ApplyFriction();
    void FallthroughPlatform();
    void WallJump(int dir);
    void Airdodge();
    void UpB() { SetVelocity(Velocity().x, -120.f); }
    void Turnaround() { SetDirection(Direction() * -1); }
    void SetStage(const StageEntity *s) { data.groundedData.stage = s; }
    void Airborne() { initAirborneData(); }
    void Respawn() { SetPosition({0.f, -800.f}); data.percent_ = 0.f; SetVelocity(0.f, 0.f); }
    
    
    void IncRot(float n) { Sprite()->rotate(n); }
    
private:
    void initAirborneData() {
        data.airborneData.jumps = 1;
        data.airborneData.walljump = true;
        data.airborneData.airdodge = true;
        data.airborneData.fastfall = false;
    }
    
    void initMslBindings();
    
public:
    // The input for this frame
    const PlayerInput* input_;
    
    bool fill = false;
    
private:
    typedef struct CharacterData {
        CharStateType actionStateT_;
        CharacterState::GameData actionStateData_;
        
        int fallthrough_ = -1;
        int ftCount_ = 0;
        
        float percent_ = 0.f;
        
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
    
    friend class PhysicsEngine;
};

#endif /* Character_hpp */