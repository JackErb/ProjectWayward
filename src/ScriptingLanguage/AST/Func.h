//
//  Func.h
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef Func_h
#define Func_h

#include "Statement.h"

#include <vector>
#include <string>

struct Func {
    Func(std::string n, Statement* s) : name(n), s(s) {}
    
    std::string name;
    Statement *s;
};

#endif /* Func_h */
