//
//  GroundedScriptState.hpp
//  SFML-App
//
//  Created by Jack Erb on 6/6/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef GroundedScriptState_hpp
#define GroundedScriptState_hpp

#include "GroundedState.hpp"

class MslInterpreter;

class GroundedScriptState : public GroundedState {
public:
    GroundedScriptState(Character *ch, std::string move) : GroundedState(ch) {
        init(move);
    }
    GroundedScriptState(Character *ch, const char* move) : GroundedState(ch) {
        init(move);
    }
    
    GroundedScriptState(Character *ch, bool rb) : GroundedState(ch) {}
    
    void ProcessInput(const PlayerInput &input) override;
    void Tick() override;
    
    void HandleCollision(const Entity &entity, VectorV pv) override;
    
    void SwitchState(CharState state) override;
    CharStateType GetStateType() const override {
        return GroundedScript;
    }
    
private:
    void init(std::string m);
};

#endif /* GroundedScriptState_hpp */
