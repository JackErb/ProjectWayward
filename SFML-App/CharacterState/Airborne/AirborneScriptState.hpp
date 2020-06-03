//
//  AirborneScriptState.hpp
//  SFML-App
//
//  Created by Jack Erb on 6/1/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef AirborneScriptState_hpp
#define AirborneScriptState_hpp

#include "AirborneState.hpp"

class MslInterpreter;

class AirborneScriptState : public AirborneState {
public:
    AirborneScriptState(Character *ch, int move) : AirborneState(ch) {
        init(move);
    }
    
    void ProcessInput(const PlayerInput &input) override;
    void Tick() override;
    
    void HandleCollision(const Entity &entity, sf::Vector2f pv) override;
    
    void SwitchState(CharState state) override;
    CharStateType GetStateType() const override {
        return Script;
    }
    
private:
    void init(int m);
};

#endif /* AirborneScriptState_hpp */
