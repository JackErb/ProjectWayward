//
//  Divide.h
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef Divide_h
#define Divide_h

#include "Expression.h"

struct Divide: public Expression {
    Divide(Expression *v1, Expression *v2) : e1(v1), e2(v2) {}
    Expression *e1, *e2;
    void accept(Visitor *v) { v->visit(this); }
};

#endif /* Divide_h */
