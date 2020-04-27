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
    DashState(Character *ch, float angle) : GroundedState(ch) {
        setDirInfluence(angle);
    }
    ~DashState() {}
    
    void HandleCollision(const Entity &entity, sf::Vector2f pv);
    void ProcessInput(const PlayerInput &input);
    void Tick();
    void SwitchState(State state);

private:
    float dirInfluence_;
    void setDirInfluence(float angle);
};

#endif /* DashState_hpp */
