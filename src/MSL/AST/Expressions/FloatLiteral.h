//
//  FloatLiteral.h
//  SFML-App
//
//  Created by Jack Erb on 6/2/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef FloatLiteral_h
#define FloatLiteral_h

#include "Expression.h"

struct FloatLiteral: public Expression {
    FloatLiteral(float f) : f(f) {}
    float f;
    void accept(Visitor *v) { v->visit(this); }
};

#endif /* FloatLiteral_h */
