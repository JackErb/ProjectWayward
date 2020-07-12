//
//  MoveLoader.cpp
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "MoveLoader.hpp"

#include "../../MSL/MslParser.hpp"
#include "../../MSL/MslScanner.hpp"
#include "ResourcePath.hpp"

#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <string>

std::unordered_map<std::string, Msl::MoveScript*> MoveLoader::LoadMoves() {
    MslScanner s;
    MslParser p;
    
    std::string files[] = {"Jab.msl", "Nair.msl", "Utilt.msl", "Ftilt.msl", "Dair.msl", "Fair.msl", "Dtilt.msl", "Uair.msl", "Fspecial.msl", "Rocket.msl"};
    std::unordered_map<std::string, Msl::MoveScript*> res;
    for (std::string file : files) {
        s.FeedInput(ResourcePath() + file);
        Msl::MoveScript *ms = p.parseProgram(&s);
        res[ms->name] = ms;
    }
    return res;
}
