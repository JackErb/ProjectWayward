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
    sf::Vector2f Velocity() const { return data.velocity_; }
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
    void NullVelocityY() { data.velocity_.y = 0; }
    void NullVelocityX() { data.velocity_.x = 0; }
    void Knockback(float angle, float kb) {
        data.velocity_ = sf::Vector2f(cos(angle) * kb, sin(angle) * kb);
    }
    void ClearHitboxIgnore() { ignoreHits.clear(); }
    void Freeze(int f) { data.freeze_ = true; data.freezeFr_ = f; }
    void Dash(float m);
    void Vector(float v = 1.f);
    void ApplyGravity(float m = 1.f);
    void ApplyVelocity() { Transform(data.velocity_); }
    void ApplyFriction();
    void FallthroughPlatform();
    void WallJump(int dir);
    void Airdodge();
    void UpB() { data.velocity_.y = -80.f; }
    void Turnaround() { SetDirection(Direction() * -1); }
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
        
        sf::Vector2f velocity_ = sf::Vector2f(0.f, 0.f);
        int fallthrough_ = -1;
        int ftCount_ = 0;
        
        float percent_ = 0.f;
        
        bool freeze_ = false;
        int freezeFr_ = 0;
        
        union {
            GroundedData groundedData;
            AirborneData airborneData;
        };
    } GameData;
    
    // Pointer to the current game data state.
    GameData data;
    GameData rollback_;
    
    // ids of entities to ignore hits from
    std::set<int> ignoreHits;
    
    CharacterState *actionState_;
    
    const Attributes attr;
    AnimMap anims_;
    
    friend class MslInterpreter;
    friend class PhysicsEngine;
};

#endif /* Character_hpp */
