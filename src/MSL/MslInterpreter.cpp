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

MslInterpreter::MslInterpreter(Character* ch) : exprRes_(ERR), ch_(ch) {
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
        data.pid = this->ch_->hitboxes[script][data.id].size();
        data.hitbox = {{params_[1].f, params_[2].f}, {params_[3].f, 0}};
        this->ch_->CreateHitbox(script, data);
    };
    
    bindings_["Hitbox_Damage"] = [=]() {
        int id = params_[0].n;
        this->ch_->hitboxes[script][id].back().dmg = params_[1].f;
    };
    
    bindings_["Hitbox_Knockback"] = [=]() {
        int id = params_[0].n;
        this->ch_->hitboxes[script][id].back().angle = (360 - (float)params_[1].n) * PI / 180.f;
        this->ch_->hitboxes[script][id].back().basekb = params_[2].f;
        this->ch_->hitboxes[script][id].back().kbscale = params_[3].f;
    };
    
    bindings_["Hitbox_Reverse"] = [=]() {
        int  id = params_[0].n;
        auto back = this->ch_->hitboxes[script][id].back();
        back.reverse = !back.reverse;
    };
    
    bindings_["Hitbox_Spawn"] = [=]() {
        if (params_.size() == 2) {
            this->ch_->SpawnHitbox(params_[0].n, params_[1].n);
        } else {
            this->ch_->SpawnHitbox(params_[0].n);
        }
    };
    
    bindings_["Hitbox_Remove"] = [=]() {
        if (params_.size() == 2) {
            this->ch_->RemoveHitbox(params_[0].n, params_[1].n);
        } else {
            this->ch_->RemoveHitbox(params_[0].n);
        }
    };
    
    bindings_["Hitbox_Clear"] = [=]() {
        this->ch_->ClearHitboxes();
    };
    
    bindings_["NullVelocity"] = [=]() {
        this->ch_->NullVelocityX();
        this->ch_->NullVelocityY();
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
