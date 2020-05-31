//
//  Var.h
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef Var_h
#define Var_h

#include "Expression.h"
#include <string>

struct Var: public Expression {
    Var(std::string n) : name(n) {}
    std::string name;
    void accept(Visitor *v) { v->visit(this); }
};

#endif /* Var_h */
