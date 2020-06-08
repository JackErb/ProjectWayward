//
//  CharacterState.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/26/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef CharacterState_hpp
#define CharacterState_hpp

#include "../../MathHelper.hpp"
#include "../../PlayerInput.hpp"
#include "../CharacterProperties.hpp"

#include <list>
#include <SFML/Graphics.hpp>

using std::list;

class Entity;
class Character;

class CharacterState {    
public:
    CharacterState(Character *ch) : character_(ch) {}
    virtual ~CharacterState() {}
    
    virtual void ProcessInput(const PlayerInput &input) = 0;
    virtual void Tick() = 0;
    
    virtual void HandleCollision(const Entity &entity, sf::Vector2f pv) = 0;
    
    virtual void SwitchState(CharState state) = 0;
    virtual CharState GetState() const = 0;
    virtual CharStateType GetStateType() const = 0;

    void SetFrame(int f) { data.frame_ = f; }
    int Frame() const { return data.frame_; }
    
protected:
    typedef struct CharacterStateData {
        int frame_;
        int type_;
        float dirInfluence_;
        bool b1;
        
    } GameData;
    CharacterStateData data;
    
    Character *character_;
    
    void setData(GameData d) {
        data = d;
    }
    
    friend class Character;
};

#endif /* CharacterState_hpp */
