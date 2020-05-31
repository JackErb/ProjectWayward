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

class Expression;

// <NAME>(<PARAM LIST>)
struct FunctionCall: public Statement {
    std::string name;
    std::vector<Expression*> params;
    
    void accept(Visitor *v) { v->visit(this); }
};

#endif /* FunctionCall_h */
