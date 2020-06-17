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
    AirdodgeState(Character *ch, float angle) : AirborneState(ch) {
        // character_->airborneData.fastfall = false;
        Airdodge();
        data.frame_ = 0;
        data.dirInfluence_ = angle;
    }
    
    AirdodgeState(Character *ch, bool rb) : AirborneState(ch) {}
    
    void ProcessInput(const PlayerInput &input) override;
    void Tick() override;
    
    void HandleCollision(const Entity &e, VectorV pv) override;
    void SwitchState(CharState state) override;
    
    CharStateType GetStateType() const override { return CharStateType::Airdodge; }
    
private:
    void Airdodge();
};

#endif /* AirdodgeState_hpp */
