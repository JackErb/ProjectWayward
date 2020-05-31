//
//  Times.h
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef Times_h
#define Times_h

#include "Expression.h"

struct Times: public Expression {
    Times(Expression *v1, Expression *v2) : e1(v1), e2(v2) {}
    Expression *e1, *e2;
    void accept(Visitor *v) { v->visit(this); }
};

#endif /* Times_h */
