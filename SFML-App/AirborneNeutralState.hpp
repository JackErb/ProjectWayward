//
//  AirborneNeutralState_hpp.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/27/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef AirborneNeutralState_hpp
#define AirborneNeutralState_hpp

#include "AirborneState.hpp"

class AirborneNeutralState : public AirborneState {
public:
    AirborneNeutralState(Character *ch) : AirborneState(ch) {}
    ~AirborneNeutralState() {}
    
    void ProcessInput(const PlayerInput &input) override;
    void Tick() override;
    void HandleCollision(const Entity &e1, sf::Vector2f) override;
    void SwitchState(Character::CState s) override;
};

#endif /* AirborneNeutralState_hpp */
