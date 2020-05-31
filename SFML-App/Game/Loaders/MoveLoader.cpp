//
//  MoveLoader.cpp
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "MoveLoader.hpp"
#include "MslParser.hpp"
#include "MslScanner.hpp"
#include "ResourcePath.hpp"

#include <filesystem>
#include <iostream>

std::unordered_map<MoveType, MoveLogic*>* MoveLoader::LoadMoves() {
    MslScanner s;
    MslParser p;
        
    s.FeedInput(resourcePath() + "NeutralAir.msl");
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
    p.parseProgram(&s);
}
