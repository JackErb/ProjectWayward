//
//  StringLiteral.h
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef StringLiteral_h
#define StringLiteral_h

#include "Expression.h"
#include <string>

struct StringLiteral: public Expression {
    StringLiteral(std::string s) : s(s) {}
    std::string s;
    void accept(Visitor *v) { v->visit(this); }
};

#endif /* StringLiteral_h */
