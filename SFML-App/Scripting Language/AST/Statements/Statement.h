//
//  Statement.h
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef Statement_h
#define Statement_h

#include "ASTVisitor.h"

struct Statement {
    virtual void accept(Visitor *v) = 0;
};

#endif /* Statement_h */
