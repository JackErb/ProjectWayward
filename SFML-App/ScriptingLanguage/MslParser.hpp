//
//  MslParser.hpp
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef MslParser_hpp
#define MslParser_hpp

#include "Msl.hpp"
#include "MoveLoader.hpp"

#include <unordered_map>
#include <string>
#include <vector>
#include <utility>

class MslScanner;
class Func;
class Expression;
class Statement;

class MslParser {
public:
    MslParser() {}
    ~MslParser() {}
    
    Msl::MoveScript* parseProgram(MslScanner *s);
    
private:
    // Recursive descent parser functions
    Statement* statement();
    std::pair<int, Statement*> switchCase();
    std::vector<Expression*> exprList();
    
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
    bool statementExpr_ = false;
    bool exprList_ = false;
    int parenDepth_ = 0;
};

#endif /* MslParser_hpp */
