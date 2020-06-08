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

MslInterpreter::MslInterpreter(Character* ch) : ch_(ch), exprRes_(ERR) {
    initializeBindings();
    for (auto it = scripts.begin(); it != scripts.end(); it++) {
        InitScript(it->first);
        CallFunction("Init");
    }
}

void MslInterpreter::initializeBindings() {
    // These are bindings that can be called in the scripting language
    bindings_["Hitbox_Circle"] = [=]() {
        HitboxData data;
        data.id = params_[0].n;
        data.hitbox = {{params_[1].f, params_[2].f}, {params_[3].f, 0}};
        this->ch_->CreateHitbox(script, data);
    };
    
    bindings_["Hitbox_Damage"] = [=]() {
        this->ch_->hitboxes[script][params_[0].n].dmg = params_[1].f;
    };
    
    bindings_["Hitbox_Knockback"] = [=]() {
        this->ch_->hitboxes[script][params_[0].n].angle = (360 - (float)params_[1].n) * PI / 180.f;
        this->ch_->hitboxes[script][params_[0].n].basekb = params_[2].f;
        this->ch_->hitboxes[script][params_[0].n].kbscale = params_[3].f;
    };
    
    bindings_["Hitbox_Reverse"] = [=]() {
        this->ch_->hitboxes[script][params_[0].n].reverse = !this->ch_->hitboxes[script][params_[0].n].reverse;
    };
    
    bindings_["Hitbox_Spawn"] = [=]() {
        this->ch_->SpawnHitbox(params_[0].n);
    };
    
    bindings_["Hitbox_Remove"] = [=]() {
        this->ch_->RemoveHitbox(params_[0].n);
    };
    
    bindings_["Hitbox_Clear"] = [=]() {
        this->ch_->ClearHitboxes();
    };
    
    
    bindings_["vector"] = [=]() {
        this->ch_->Vector();
    };
    
    bindings_["gravity"] = [=]() {
        float f;
        if (this->params_.size() == 0) { f = 1.f; }
        else { f = this->params_[0].f; }
        this->ch_->ApplyGravity(f);
    };
    
    bindings_["quit"] = [=]() {
        switch (this->ch_->actionState_->GetState()) {
            case GROUNDED:
                this->ch_->SetActionState(new NeutralState(ch_));
                break;
            case AIRBORNE:
                this->ch_->SetActionState(new AirborneNeutralState(ch_));
                break;
        }
    };
    
    bindings_["rotate"] = [=]() {
        this->ch_->IncRot(10);
    };
    
    bindings_["rotate0"] = [=]() {
        this->ch_->Sprite()->setRotation(0);
    };
    
    bindings_["setState"] = [=]() {
        std::string state = this->params_[0].str;
        if (state.compare("LandingLag") == 0) {
            int f = this->params_[1].n;
            this->ch_->SetActionState(new LandingLagState(ch_, f));
        }
    };
}

void MslInterpreter::InitScript(string move) {
    // Initialize global variables/functions
    // Reset state of interpreter entirely
    script = move;
    ch_->move = move;
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
