//
//  FunctionCall.h
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef FunctionCall_h
#define FunctionCall_h

#include "Statement.h"
#include <string>
#include <vector>

struct Expression;

// <NAME>(<PARAM LIST>)
struct FunctionCall: public Statement {
    FunctionCall(std::string n, std::vector<Expression*> p) : name(n), params(p) {}
    
    std::string name;
    std::vector<Expression*> params;
    
    void accept(Visitor *v) { v->visit(this); }
};

#endif /* FunctionCall_h */
