//
//  SwitchStatement.h
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef SwitchStatement_h
#define SwitchStatement_h

#include "Statement.h"
#include <unordered_map>

struct Expression;

// NOTE: Is only valid for integers
// switch <EXPR>
//    (cases...)
struct SwitchStatement: public Statement {
    SwitchStatement(Expression *e) : expr(e), cases() {}
    Expression *expr;
    std::unordered_map<int, Statement*> cases;
    bool defBranch = false;
    Statement *def;
    
    void accept(Visitor *v) { v->visit(this); }
};

#endif /* SwitchStatement_h */
