//
//  CharacterState.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/26/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef CharacterState_hpp
#define CharacterState_hpp

#include "../../../MathHelper/MathHelper.h"
#include "../../PlayerInput.hpp"
#include "../CharacterProperties.hpp"

#include <list>

using std::list;

class Entity;
class Character;

class CharacterState {    
public:
    CharacterState(Character *ch) : character(ch) {}
    virtual ~CharacterState() {}
    
    virtual void ProcessInput(const PlayerInput &input) = 0;
    virtual void Tick() = 0;
    
    virtual void HandleCollision(const Entity &entity, VectorV pv) = 0;
    
    virtual void SwitchState(CharState state) = 0;
    virtual CharState GetState() const = 0;
    virtual CharStateType GetStateType() const = 0;

    void SetFrame(int f) { data.frame = f; }
    int Frame() const { return data.frame; }
    
protected:
    typedef struct CharacterStateData {
        int frame;
        int type;
        fpoat dirInfluence;
        bool b1;
		fpoat angle, kb;
        
    } GameData;
    CharacterStateData data;
    
    Character *character;
    
    void setData(GameData d) {
        data = d;
    }
    
    friend class Character;
};

#endif /* CharacterState_hpp */
