//
//  MslInterpreter.cpp
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "MslInterpreter.hpp"
#include "AST/Statements/Statement.h"

#include "AST/Func.h"
#include "../Game/Loaders/MoveLoader.hpp"
#include "MslScanner.hpp"
#include "../Game/Character/Character.hpp"
#include "../Game/MathHelper.hpp"
#include "../Game/Character/CharacterState/Airborne/AirborneNeutralState.hpp"
#include "../Game/Character/CharacterState/Grounded/NeutralState.hpp"
#include "../Game/Character/CharacterState/Grounded/LandingLagState.hpp"

#include <iostream>
#include <unordered_map>
#include <string>

using namespace std::chrono;

using std::unordered_map;
using std::string;
using std::cerr;
using std::cout;
using std::cerr;
using std::endl;

const unordered_map<string, Msl::MoveScript*> MslInterpreter::scripts = MoveLoader::LoadMoves();

MslInterpreter::MslInterpreter() : exprRes_(ERR) {
    
}

MslInterpreter::~MslInterpreter() {
    
}

void MslInterpreter::Init(std::vector<std::string> scripts) {
    for (string s : scripts) {
        InitScript(s);
        CallFunction("Init");
    }
}

void MslInterpreter::InitScript(string move) {
    // Initialize global variables/functions
    // Reset state of interpreter entirely
    script = move;
}

void MslInterpreter::PreTick(int frame) {
    // Update Msl vars
    time = high_resolution_clock::now();
    vals_["frame"] = ExprRes(frame);
}

void MslInterpreter::ProcessInput() {
    CallFunction("ProcessInput");
}

void MslInterpreter::Tick() {
    CallFunction("Tick");
    
    auto now = high_resolution_clock::now();
    // cerr << "Total time " << duration_cast<microseconds>(now - time).count() << endl;
}

void MslInterpreter::CallFunction(string name) {
    // Run function
    Func *f = scripts.find(script)->second->funcs[name];
    f->s->accept(this);
}
