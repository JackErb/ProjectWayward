//
//  Minus.h
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef Minus_h
#define Minus_h

#include "Expression.h"

// <E1> - <E2>
struct Minus: public Expression {
    Minus(Expression *v1, Expression *v2) : e1(v1), e2(v2) {}
    Expression *e1, *e2;
    void accept(Visitor *v) { v->visit(this); }
};

#endif /* Minus_h */
