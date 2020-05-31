//
//  Block.h
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef Block_h
#define Block_h

#include "Statement.h"
#include <vector>

// Contains a list of statements
struct Block: public Statement {
    std::vector<Statement*> ls;
    
    void accept(Visitor *v) { v->visit(this); }
}

#endif /* Block_h */
