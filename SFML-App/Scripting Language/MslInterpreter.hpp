//
//  MslInterpreter.hpp
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef MslInterpreter_hpp
#define MslInterpreter_hpp

#include <unordered_map>
#include <vector>
#include <string>
#include <functional>

#include "Msl.hpp"
#include "MoveLoader.hpp"
#include "ASTVisitor.h"

class Character;

class MslInterpreter: public Visitor {
public:
    MslInterpreter(Character *ch, MoveFuncs *ls);
    ~MslInterpreter() {}
    
    void initializeGlobalFunctions();
    
public:
    // AST Visitor functions
    void Call(std::string name);
    
    void visit(AssignStatement *s);
    void visit(SwitchStatement *s);
    void visit(Block *s);
    void visit(FunctionCall *s);
    
    void visit(Plus *e);
    void visit(Minus *e);
    void visit(Var *e);
    void visit(IntLiteral *e) ;
    void visit(StringLiteral *e);
    void visit(Times *e);
    void visit(Divide *e);
    
private:
    // Variables used while visiting AST
    typedef enum ResType {
        INT, STRING, ERR
    } ResType;
    
    // Whenever an Expression node is visited, it saves
    // its return type and associated value
    ResType exprType;
    std::string exprStr;
    int exprInt;
    
    
private:
    Character *ch_;
    
    std::unordered_map<std::string, int> vals_;
    MoveFuncs *funcs_;
    std::unordered_map<std::string, std::function<void(void)>> bindings_;
};

#endif /* MslInterpreter_hpp */
