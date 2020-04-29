//
//  AirdodgeState.hpp
//  SFML-App
//
//  Created by Jack Erb on 4/28/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef AirdodgeState_hpp
#define AirdodgeState_hpp

#include "AirborneState.hpp"

class AirdodgeState : public AirborneState {
public:
    AirdodgeState(Character *ch, float angle) : AirborneState(ch), angle_(angle) {
        character_->airborneData.fastfall = false;
    }
    
    void ProcessInput(const PlayerInput &input) override;
    void Tick() override;
    void HandleCollision(const Entity &e, sf::Vector2f pv) override;
    void SwitchState(Character::CState state) override;
    
private:
    int frame_ = 0;
    int angle_;
};

#endif /* AirdodgeState_hpp */
