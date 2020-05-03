//
//  DashState.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/27/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef DashState_hpp
#define DashState_hpp

#include "GroundedState.hpp"
#include <list>

class DashState : public GroundedState {
public:
    DashState(Character *ch, float angle, float x) : GroundedState(ch) {
        setDirInfluence(angle, x);
        frame_ = 0;
    }
    ~DashState() {}
    
    void HandleCollision(const Entity &entity, sf::Vector2f pv) override;
    void ProcessInput(const PlayerInput &input) override;
    void Tick() override;
    void SwitchState(Character::CState state) override;
    
    void RollbackTick() override {
        CharacterState::RollbackTick();
        
        GameData *copy = new GameData();
        *copy = data;
        rollback_.push_front(copy);
        
        if (rollback_.size() > NetworkController::RollbackFrames) {
            delete rollback_.back();
            rollback_.pop_back();
        }
    }
    
    void Rollback(int frames) override {
        CharacterState::Rollback(frames);
        
        for (int i = 1; i < frames-1; i++) {
            delete rollback_.front();
            rollback_.pop_front();
        }
        data = *rollback_.front();
        rollback_.pop_front();
    }
    
    CharacterStateType GetStateType() const override { return Dash; }

private:
    typedef struct GameData {
        float dirInfluence_;
    } GameData;
    
    GameData data;
    std::list<GameData*> rollback_;
    
    // Returns false if the angle switched from positive to negative or vice versa
    bool setDirInfluence(float angle, float x);
};

#endif /* DashState_hpp */
