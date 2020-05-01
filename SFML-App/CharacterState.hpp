//
//  CharacterState.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/26/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef CharacterState_hpp
#define CharacterState_hpp

#include "Character.hpp"
#include "MathHelper.hpp"
#include <list>

using std::list;

class Character;

class CharacterState {
public:
    CharacterState(Character *ch) : character_(ch) {}
    virtual ~CharacterState() {}
    
    virtual void HandleCollision(const Entity &entity, sf::Vector2f pv) = 0;
    virtual void ProcessInput(const PlayerInput &input) = 0;
    virtual void Tick() = 0;
    virtual void SwitchState(Character::CState state) = 0;
    virtual Character::CState GetState() const = 0;
    virtual CharacterStateType GetStateType() const = 0;
    
    virtual void RollbackTick() {
        rollback_.push_front(frame_);
        if (rollback_.size() > rbFrames) {
            rollback_.pop_back();
        }
    }
    
    virtual void Rollback() {
        frame_ = rollback_.front();
        rollback_.pop_front();
    }
    
    void SetFrame(int f) { frame_ = f; }
    int Frame() const { return frame_; }
    
protected:
    list<int> rollback_;
    int rbFrames = 30;
    
    Character *character_;
    int frame_ = 0;
    
    friend class Character;
};

#endif /* CharacterState_hpp */
