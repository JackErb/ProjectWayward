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
#include <string>

class MslInterpreter;

class AirborneScriptState : public AirborneState {
public:
    AirborneScriptState(Character *ch, std::string move) : AirborneState(ch) {
        init(move);
    }
    AirborneScriptState(Character *ch, const char* move) : AirborneState(ch) {
        init(move);
    }
    AirborneScriptState(Character *ch, bool rb) : AirborneState(ch) {}
    
    void ProcessInput(const PlayerInput &input) override;
    void Tick() override;
    
    void HandleCollision(const Entity &entity, VectorV pv) override;
    
    void SwitchState(CharState state) override;
    CharStateType GetStateType() const override {
        return AirborneScript;
    }
    
private:
    void init(std::string m);
};

#endif /* AirborneScriptState_hpp */
