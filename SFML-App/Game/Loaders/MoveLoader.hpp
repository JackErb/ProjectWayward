//
//  MoveLoader.hpp
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef MoveLoader_hpp
#define MoveLoader_hpp

#include "Msl.hpp"

#include <unordered_map>
#include <string>
#include <vector>

class Func;

class MoveLoader {
public:
    static std::unordered_map<std::string, Msl::MoveScript*> LoadMoves();
};

#endif /* MoveLoader_hpp */
