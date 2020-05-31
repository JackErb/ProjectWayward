//
//  MslScanner.hpp
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef MslScanner_hpp
#define MslScanner_hpp

#include <string>
#include <unordered_map>
#include <fstream>
#include <istream>

#include "Msl.hpp"

class MslScanner {
public:
    MslScanner() {}
    
    void FeedInput(std::string filename);
    Msl::Token NextToken();
    
    std::string getId();
    int getIntLiteral();
    std::string getStringLiteral();
    
private:
    void nextLine();
    Msl::Token getToken(std::string s);
    
private:
    std::ifstream file_;
    std::string line_ = "";
    bool eof_ = false;
    
    std::string str_;
    int n_;
};

#endif /* MslScanner_hpp */
