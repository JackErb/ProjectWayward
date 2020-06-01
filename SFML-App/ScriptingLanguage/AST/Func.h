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
    Func(std::string n, std::vector<Statement*> ls) : name(n), statements(ls) {}
    
    std::string name;
    std::vector<Statement*> statements;
};

#endif /* Func_h */
