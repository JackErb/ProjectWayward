//
//  Negate.h
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef Negate_h
#define Negate_h

#include "Expression.h"

struct Negate: public Expression {
    Negate(Expression *e) : e(e) {}
    Expression *e;
    void accept(Visitor *v) { v->visit(this); }
};

#endif /* Negate_h */
