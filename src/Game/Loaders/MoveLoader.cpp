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
    
	std::string files[] = {"Jab.msl", "Nair.msl", "Utilt.msl", "Ftilt.msl", "Dair.msl", "Fair.msl", "Dtilt.msl", "Uair.msl"};
    std::unordered_map<std::string, Msl::MoveScript*> res;
    for (std::string file : files) {
        s.FeedInput(ResourcePath() + "Moves/" + file);
        Msl::MoveScript *ms = p.parseProgram(&s);
        res[ms->name] = ms;
    }
    return res;
}

/*Msl::Token t = s.NextToken();
while (t != Msl::EOF_) {
    if (t == Msl::IDENTIFIER) {
        std::cerr << s.getId() << std::endl;
    } else if (t == Msl::INT) {
        std::cerr << s.getIntLiteral() << std::endl;
    } else {
        std::cerr << Msl::toString(t) << " " << std::endl;
    }
    t = s.NextToken();
}*/
