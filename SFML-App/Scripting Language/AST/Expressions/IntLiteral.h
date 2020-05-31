//
//  IntLiteral.h
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef IntLiteral_h
#define IntLiteral_h

#include "Expression.h"

struct IntLiteral: public Expression {
    IntLiteral(int n) : n(n) {}
    int n;
    void accept(Visitor *v) { v->visit(this); }
};

#endif /* IntLiteral_h */
