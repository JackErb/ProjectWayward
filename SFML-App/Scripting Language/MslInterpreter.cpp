//
//  MslInterpreter.cpp
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "MslInterpreter.hpp"
#include "MslScanner.hpp"
#include "Character.hpp"
#include "AirborneNeutralState.hpp"

#include <iostream>

using std::string;
using std::cout;
using std::cerr;
using std::endl;

MslInterpreter::MslInterpreter(Character* ch, MoveFuncs *ls) : ch_(ch), funcs_(ls) {
    initializeGlobalFunctions();
}

void MslInterpreter::initializeGlobalFunctions() {
    bindings_["jump"] = [=]() {
        this->ch_->SetActionState(new AirborneNeutralState(ch_));
        this->ch_->Jump(JUP, true);
    };
    // To call a function:
    // funcs["jump"]();
}

void MslInterpreter::Call(std::string name) {
    auto f = funcs_->find(name);
    if (f != funcs_.end()) {
    }
}

void MslInterpreter::visit(AssignStatement *s) {
    
}

void MslInterpreter::visit(SwitchStatement *s) {
    
}

void MslInterpreter::visit(Block *s) {
    
}

void MslInterpreter::visit(FunctionCall *s) {
    
}

void MslInterpreter::visit(Plus *e) {
    
}

void MslInterpreter::visit(Minus *e) {
    
}

void MslInterpreter::visit(Var *e) {
    
}

void MslInterpreter::visit(IntLiteral *e) {
    
}

void MslInterpreter::visit(StringLiteral *e) {
    
}

void MslInterpreter::visit(Times *e) {
    
}

void MslInterpreter::visit(Divide *e) {
    
}
