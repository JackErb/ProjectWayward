//
//  MslInterpreter.cpp
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "MslInterpreter.hpp"

#include "Func.h"
#include "MoveLoader.hpp"
#include "MslScanner.hpp"
#include "Character.hpp"
#include "AirborneNeutralState.hpp"
#include "LandingLagState.hpp"

#include "Expression.h"
#include "AssignStatement.h"
#include "FunctionCall.h"
#include "Plus.h"
#include "Minus.h"
#include "Times.h"
#include "Divide.h"
#include "Block.h"
#include "Var.h"
#include "IntLiteral.h"
#include "StringLiteral.h"
#include "FloatLiteral.h"
#include "SwitchStatement.h"

#include <iostream>

using namespace std::chrono;

using std::string;
using std::cerr;
using std::cout;
using std::cerr;
using std::endl;

const vector<Msl::MoveScript*> MslInterpreter::scripts = MoveLoader::LoadMoves();

MslInterpreter::MslInterpreter(Character* ch) : ch_(ch), exprRes_(ERR) {
    initializeBindings();
}

void MslInterpreter::initializeBindings() {
    // These are bindings that can be called in the scripting language
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
        this->ch_->SetActionState(new AirborneNeutralState(ch_));
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
    
    bindings_["circleHitbox"] = [=]() {
        HitboxData data;
        data.hitbox = {{params_[0].f, params_[1].f}, {params_[2].f`, 0}};
        ch_->CreateHitbox(data);
    };
    
    bindings_["removeHitbox"] = [=]() {
        ch_->RemoveHitbox(params_[0].n);
    };
    
    bindings_["clearHitboxes"] = [=]() {
        ch_->ClearHitboxes();
    };
}

void MslInterpreter::InitScript(int move) {
    // Initialize global variables/functions
    // Reset state of interpreter entirely
    CallFunction("Init");
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
    cerr << "Total time " << duration_cast<microseconds>(now - time).count() << endl;
}

void MslInterpreter::CallFunction(string name) {
    // Check if function exists
    auto it = scripts[0]->find(name);
    if (it == scripts[0]->end()) {
        cerr << "undefined script function " << name << "()" << endl;
        return;
    }
    
    // Run function
    Func *f = it->second;
    f->s->accept(this);
}

bool MslInterpreter::err(const ExprRes &e1) {
    return e1.type == ERR;
}

bool MslInterpreter::eq(const ExprRes &e1, const BaseType &t) {
    return e1.type == t;
}

void MslInterpreter::visit(Func *s) {
    s->s->accept(this);
}

void MslInterpreter::visit(AssignStatement *s) {
    s->expr->accept(this);
    ExprRes r = ExprRes(exprRes_);
    vals_[s->var] = r;
}

void MslInterpreter::visit(SwitchStatement *s) {
    s->expr->accept(this);
    if (err(exprRes_)) {
        return;
    }
    
    if (eq(exprRes_, INT)) {
        auto c = s->cases.find(exprRes_.n);
        if (c != s->cases.end()) {
            // run the case branch
            c->second->accept(this);
        } else if (s->defBranch){
            // run the default branch, if it exists
            s->def->accept(this);
        }
    } else {
        cerr << "switch must be applied to number" << endl;
        exprRes_ = ExprRes(ERR);
    }
}

void MslInterpreter::visit(Block *s) {
    for (Statement *ss : s->ls) ss->accept(this);
}

void MslInterpreter::visit(FunctionCall *s) {
    auto f = bindings_.find(s->name);
    if (f != bindings_.end()) {
        params_.clear();
        for (Expression *e : s->params) {
            e->accept(this);
            params_.push_back(ExprRes(exprRes_));
        }
        f->second();
    } else {
        cerr << "undefined function " << s->name << "()" << endl;
    }
}

void MslInterpreter::visit(Plus *e) {
    e->e1->accept(this);
    ExprRes r1 = exprRes_;
    if (err(r1)) return;
    
    e->e2->accept(this);
    ExprRes r2 = exprRes_;
    if (err(r2)) return;
    
    if (eq(r1, INT) && eq(r2, INT)) {
        exprRes_ = ExprRes(INT);
        exprRes_.n = r1.n + r2.n;
    } else {
        cerr << "addition between two non-numbers" << endl;
        exprRes_ = ExprRes(ERR);
    }
}

void MslInterpreter::visit(Minus *e) {
    e->e1->accept(this);
    ExprRes r1 = exprRes_;
    if (err(r1)) return;
    
    e->e2->accept(this);
    ExprRes r2 = exprRes_;
    if (err(r2)) return;
    
    if (eq(r1, INT) && eq(r2, INT)) {
        exprRes_ = ExprRes(INT);
        exprRes_.n = r1.n - r2.n;
    } else {
        cerr << "subtraction between two non-numbers" << endl;
        exprRes_ = ExprRes(ERR);
    }
}

void MslInterpreter::visit(Var *e) {
    auto v = vals_.find(e->name);
    if (v != vals_.end()) {
        exprRes_ = v->second;
    } else {
        cerr << "undefined var " << e->name << endl;
        exprRes_ = ExprRes(ERR);
    }
}

void MslInterpreter::visit(IntLiteral *e) {
    exprRes_ = ExprRes(INT);
    exprRes_.n = e->n;
}

void MslInterpreter::visit(StringLiteral *e) {
    exprRes_ = ExprRes(STRING);
    exprRes_.str = e->s;
}

void MslInterpreter::visit(FloatLiteral *e) {
    exprRes_ = ExprRes(FLOAT);
    exprRes_.f = e->f;
}

void MslInterpreter::visit(Times *e) {
    e->e1->accept(this);
    ExprRes r1 = exprRes_;
    if (err(r1)) return;
    
    e->e2->accept(this);
    ExprRes r2 = exprRes_;
    if (err(r2)) return;
    
    if (eq(r1, INT) && eq(r2, INT)) {
        exprRes_ = ExprRes(INT);
        exprRes_.n = r1.n * r2.n;
    } else {
        cerr << "multiplication between two non-numbers" << endl;
        exprRes_ = ExprRes(ERR);
    }
}

void MslInterpreter::visit(Divide *e) {
    e->e1->accept(this);
    ExprRes r1 = exprRes_;
    if (err(r1)) return;
    
    e->e2->accept(this);
    ExprRes r2 = exprRes_;
    if (err(r2)) return;
    
    if (eq(r1, INT) && eq(r2, INT)) {
        exprRes_ = ExprRes(INT);
        exprRes_.n = r1.n / r2.n;
    } else {
        cerr << "division between two non-numbers" << endl;
        exprRes_ = ExprRes(ERR);
    }
}
