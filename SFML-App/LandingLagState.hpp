//
//  LandingLagState.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/27/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef LandingLagState_hpp
#define LandingLagState_hpp

#include "GroundedState.hpp"

class LandingLagState : public GroundedState {
public:
    LandingLagState(Character *ch, Entity* const stage, int frames) : GroundedState(ch, stage), frames_(frames) {}
    ~LandingLagState() {}
    
    void ProcessInput(const PlayerInput &input) override;
    void Tick() override;
    void HandleCollision(const Entity& e1, sf::Vector2f) override;
    void SwitchState(State s) override {}
    
private:
    int frames_;
};

#endif /* LandingLagState_hpp */
