//
//  MslInterpreter_visitor.cpp
//  SFML-App
//
//  Created by Jack Erb on 6/5/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "MslInterpreter.hpp"

#include "MslScanner.hpp"

#include "AST/Func.h"
#include "AST/ASTPrintVisitor.h"
#include "AST/Expressions/Plus.h"
#include "AST/Expressions/Minus.h"
#include "AST/Expressions/Times.h"
#include "AST/Expressions/Divide.h"
#include "AST/Expressions/IntLiteral.h"
#include "AST/Expressions/StringLiteral.h"
#include "AST/Expressions/Var.h"
#include "AST/Expressions/Negate.h"
#include "AST/Statements/AssignStatement.h"
#include "AST/Statements/FunctionCall.h"
#include "AST/Statements/Block.h"
#include "AST/Statements/SwitchStatement.h"

#include <iostream>

using std::cerr;
using std::endl;
using std::cout;

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
    exprRes_ = ExprRes(e->n);
}

void MslInterpreter::visit(StringLiteral *e) {
    exprRes_ = ExprRes(e->s);
}

void MslInterpreter::visit(FloatLiteral *e) {
    exprRes_ = ExprRes(e->f);
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

void MslInterpreter::visit(Negate *e) {
    cout << "Here" << endl;
    e->e->accept(this);
    if (err(exprRes_)) return;
    
    if (eq(exprRes_, INT)) {
        exprRes_.n *= -1;
    } else if (eq(exprRes_, FLOAT)) {
        cout << "HERE" << endl;
        exprRes_.f *= -1;
    } else {
        cerr << "negation of non-number" << endl;
        exprRes_ = ExprRes(ERR);
    }
}
