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
    
    CharacterStateType GetStateType() const override { return Dash; }

private:
    float dirInfluence_;
    // Returns false if the angle switched from positive to negative or vice versa
    bool setDirInfluence(float angle, float x);
};

#endif /* DashState_hpp */
