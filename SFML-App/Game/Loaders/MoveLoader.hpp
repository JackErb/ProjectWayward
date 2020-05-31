//
//  MoveLoader.hpp
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef MoveLoader_hpp
#define MoveLoader_hpp

#include <unordered_map>
#include <string>

#include "CharacterProperties.hpp"

class Func;

typedef std::unordered_map<std::string, Func*> MoveLogic;
typedef std::unordered_map<MoveType, MoveLogic*> MoveFuncs;

class MoveLoader {
public:
    static MoveFuncs* LoadMoves();
};

#endif /* MoveLoader_hpp */
