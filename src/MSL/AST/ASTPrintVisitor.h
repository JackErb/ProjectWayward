//
//  ASTPrintVisit.h
//  SFML-App
//
//  Created by Jack Erb on 5/31/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef ASTPrintVisit_h
#define ASTPrintVisit_h

#include "Func.h"
#include "ASTPrintVisitor.h"
#include "Expressions/Plus.h"
#include "Expressions/Minus.h"
#include "Expressions/Times.h"
#include "Expressions/Divide.h"
#include "Expressions/IntLiteral.h"
#include "Expressions/StringLiteral.h"
#include "Expressions/FloatLiteral.h"
#include "Expressions/Var.h"
#include "Expressions/Negate.h"
#include "Statements/AssignStatement.h"
#include "Statements/FunctionCall.h"
#include "Statements/Block.h"
#include "Statements/SwitchStatement.h"

#include <iostream>
#include <string>

using std::cerr;
using std::cout;
using std::endl;
using std::string;

class ASTPrintVisitor: public Visitor {
private:
    int depth;
    
    void println() {
        cout << endl;
    }
    
    void indent() {
        for (int i = 0; i < depth; i++) cout << "  ";
    }
    
    void print(string s) {
        cout << s;
    }
    
public:
    void visit(Func *s) {
        depth = 0;
        print("func " + s->name + ":");
        println();
        depth++;
        s->s->accept(this);
        depth--;
    }
    
    void visit(AssignStatement *s) {
        indent();
        print(s->var + " = ");
        s->expr->accept(this);
        println();
    }
    
    void visit(SwitchStatement *s) {
        indent();
        print("switch ");
        s->expr->accept(this);
        print(":");
        println();
        depth++;
        for (auto it = s->cases.begin(); it != s->cases.end(); it++) {
            indent();
            print("case " + std::to_string(it->first) + ":");
            println();
            
            depth++;
            it->second->accept(this);
            depth--;
        }
        
        if (s->defBranch) {
            indent();
            print("default:");
            println();
            
            depth++;
            s->def->accept(this);
            depth--;
        }
        
        depth--;
    }
    
    void visit(Block *s) {
        for (Statement *ss : s->ls) ss->accept(this);
    }
    
    void visit(FunctionCall *s) {
        indent();
        print(s->name + "(");
        for (int i = 0; i < s->params.size(); i++) {
            s->params[i]->accept(this);
            if (i != s->params.size() - 1) print(", ");
        }
        print(")");
        println();
    }
    
    void visit(Plus *e) {
        print("(");
        e->e1->accept(this);
        print(" + ");
        e->e2->accept(this);
        print(")");
    }
    
    void visit(Minus *e) {
        print("(");
        e->e1->accept(this);
        print(" - ");
        e->e2->accept(this);
        print(")");
    }
    
    void visit(Var *e) {
        print(e->name);
    }
    
    void visit(IntLiteral *e) {
        print(std::to_string(e->n));
    }
    
    void visit(StringLiteral *e) {
        print("\"");
        print(e->s);
        print("\"");
    }
    
    void visit(FloatLiteral *e) {
        print(std::to_string(e->f));
    }
    
    void visit(Times *e) {
        print("(");
        e->e1->accept(this);
        print(" * ");
        e->e2->accept(this);
        print(")");
    }
    
    void visit(Divide *e) {
        print("(");
        e->e1->accept(this);
        print(" / ");
        e->e2->accept(this);
        print(")");
    }
    
    void visit(Negate *e) {
        print(" - ");
        e->e->accept(this);
    }
};

#endif /* ASTPrintVisit_h */
