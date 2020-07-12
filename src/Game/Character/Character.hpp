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
        fpoat GroundAccel = fpoat(0,2500);
		fpoat MaxGroundSpeed = fpoat(30,0);
		fpoat GroundFriction = fpoat(0,8700);
           
		fpoat AirAccel = fpoat(3,5000);
		fpoat MaxAirSpeed = fpoat(38,0);
		fpoat AirFriction = fpoat(0,9100);
           
		fpoat Gravity = fpoat(3, 4000);
		fpoat MaxFallSpeed = fpoat(40,0);
		fpoat FastFallSpeed = fpoat(70,0);
        
		fpoat DoubleJump = fpoat(65,0,true);
		fpoat WallJump = fpoat(60,0,true);
		fpoat FullhopJump = fpoat(63,0,true);
		fpoat ShorthopJump = fpoat(40,0,true);
        
		fpoat AirdodgeVelocity = fpoat(60,0);
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
    Character(int id, const VectorV &position);
    ~Character();
    
    /* Setup */
    void SetAnimMap(AnimMap am) {
        anims = am;
    }
    
    
    /* Get Methods */
    EntityType Type() const override { return CHARACTER; }
    TextureV* GetTexture(const string& state, int frame) { return anims[state][frame]; }
    const StageEntity* Stage() const { return data.groundedData.stage; }
    int Jumps() const { return data.airborneData.jumps; }
    bool IsFastFalling() const  { return data.airborneData.fastfall; }
    bool HasAirdodge() const { return data.airborneData.airdodge; }
    fpoat Percent() const { return data.percent; }
    
    
    /* Game Processing */
    void ProcessInput(const PlayerInput &input);
    void Tick() override;
    void HandleCollision(const Entity &entity, const VectorV& pv) override;
    bool HandleHit(const Entity *e, int f, const HitboxData& hd) override;
    
    void RollbackTick() override;
    void Rollback() override;
    
    /* Methods involving action, and should only be called by the current actionState_
     * ALL OF THESE METHODS NEED TO BE ABLE TO ROLL BACK */
    void SetActionState(CharacterState *s);
    void Jump(JumpType type, bool fullhop);
    void FastFall();
    void Knockback(const fpoat& angle, const fpoat& kb) {
        SetVelocity(fpcos(angle) * kb, fpsin(angle) * kb);
    }
    void Dash(const fpoat& m);
    void Vector();
    void ApplyGravity(const fpoat& m = fpoat(1,0));
    void ApplyFriction();
    void FallthroughPlatform();
    void WallJump(int dir);
    void Airdodge();
    void Turnaround() { SetDirection(Direction() * -1); }
    void SetStage(const StageEntity *s) { data.groundedData.stage = s; }
    void Airborne() { initAirborneData(); }
    void Respawn() { 
		SetPosition({FixedPoint::ZERO, fpoat(800,0,true)});
		data.percent = fpoat(0); SetVelocity(FixedPoint::ZERO, FixedPoint::ZERO);
	}
    
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
    const PlayerInput* input;
    
    bool fill = false;
    
private:
    typedef struct CharacterData {
        CharStateType stateType;
        CharacterState::GameData stateData;
        
        int ft = -1;
        int ftCount = 0;
        
        fpoat percent = fpoat(0);
                
        union {
            GroundedData groundedData;
            AirborneData airborneData;
        };
    } GameData;
    
    CharacterState *state;
    
    // Current game data state.
    GameData data;
    GameData rollback;
    
    const Attributes attr;
    AnimMap anims;
    
    friend class PhysicsEngine;
};

#endif /* Character_hpp */
