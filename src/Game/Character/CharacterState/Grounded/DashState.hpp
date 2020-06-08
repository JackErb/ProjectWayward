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
#include "../../../PlayerInput.hpp"
#include <list>

class DashState : public GroundedState {
public:
    DashState(Character *ch) : GroundedState(ch) {
        init();
        data.frame_ = 0;
    }
    DashState(Character *ch, bool rb) : GroundedState(ch) {}
    
    ~DashState() {}
    
    void ProcessInput(const PlayerInput &input) override;
    void Tick() override;
    void HandleCollision(const Entity &entity, sf::Vector2f pv) override;
    
    void SwitchState(CharState state) override;
    CharStateType GetStateType() const override { return Dash; }

private:
    void init();
    
    // Returns false if the angle switched from positive to negative or vice versa
    bool setDirInfluence();
};

#endif /* DashState_hpp */
