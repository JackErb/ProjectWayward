//
//  MslParser.hpp
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef MslParser_hpp
#define MslParser_hpp

#include <unordered_map>
#include <string>

#include "Msl.hpp"

class MslScanner;
class Func;
class Expression;
class Statement;

typedef std::unordered_map<std::string, Func*> MoveLogic;

class MslParser {
public:
    MslParser() {}
    ~MslParser() {}
    
    MoveLogic* parseProgram(MslScanner *s);
    
private:
    // Recursive descent parser functions
    Statement* statement();
    Expression* expr();
    Expression* exprtail(Expression *e1);
    Expression* term();
    Expression* termtail(Expression *e1);
    Expression* factor();
    
    bool assertToken(Msl::Token exp, Msl::Token act);
    bool assertNext(Msl::Token exp);
    
    void error(std::string str);
    
    
private:
    MslScanner *s_;
    
    Msl::Token curr_;
    bool err_ = false;
};

#endif /* MslParser_hpp */
