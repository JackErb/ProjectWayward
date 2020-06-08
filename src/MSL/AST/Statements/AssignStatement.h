//
//  AssignStatement.h
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef AssignStatement_h
#define AssignStatement_h

#include "Statement.h"
#include <string>

struct Expression;

// var <VAR> = <EXPR>
struct AssignStatement: public Statement {
    AssignStatement(std::string v, Expression *e) : var(v), expr(e) {}
    std::string var;
    Expression *expr;
    
    void accept(Visitor *v) { v->visit(this); }
};

#endif /* AssignStatement_h */
