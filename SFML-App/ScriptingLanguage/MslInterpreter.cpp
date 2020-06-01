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
#include "SwitchStatement.h"

#include <iostream>

using std::string;
using std::cerr;
using std::cout;
using std::cerr;
using std::endl;

MslInterpreter::MslInterpreter(Character* ch) : ch_(ch), exprRes_(ERR) {
    scripts_ = MoveLoader::LoadMoves();
    initializeGlobalFunctions();
}

void MslInterpreter::initializeGlobalFunctions() {
    bindings_["jump"] = [=]() {
        this->ch_->SetActionState(new AirborneNeutralState(ch_));
        this->ch_->Jump(JUP, true);
    };
}

void MslInterpreter::InitScript(int move) {
    // Initialize global variables/functions
    // Reset state of interpreter entirely
}

void MslInterpreter::ProcessInput() {
    // Update Msl vars (input, frame)
    
    CallFunction("ProcessInput");
}

void MslInterpreter::Tick() {
    CallFunction("Tick");
}

void MslInterpreter::CallFunction(string name) {
    Func *f = scripts_[0]->find(name)->second;
    for (Statement *s : f->statements) {
        s->accept(this);
    }
}

bool MslInterpreter::err(const ExprRes &e1) {
    return e1.type == ERR;
}

bool MslInterpreter::eq(const ExprRes &e1, const BaseType &t) {
    return e1.type == t;
}

void MslInterpreter::visit(Func *s) {
    for (Statement *ss : s->statements) ss->accept(this);
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
