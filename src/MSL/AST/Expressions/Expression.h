//
//  Expression.h
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef Expression_h
#define Expression_h

#include "../ASTVisitor.h"

struct Expression {
    virtual void accept(Visitor *v) = 0;
};

#endif /* Expression_h */
