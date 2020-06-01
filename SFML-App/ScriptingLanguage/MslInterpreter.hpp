//
//  MslInterpreter.hpp
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef MslInterpreter_hpp
#define MslInterpreter_hpp

#include "MoveLoader.hpp"
#include "Msl.hpp"
#include "ASTVisitor.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <functional>

class Character;

class MslInterpreter: public Visitor {
public:
    MslInterpreter(Character *ch);
    ~MslInterpreter() {}
    
    void initializeGlobalFunctions();
    
public:
    // AST Visitor functions
    void InitScript(int move);
    void ProcessInput();
    void Tick();
    void CallFunction(std::string name);
    
    void visit(Func *s);
    
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
    typedef enum Type {
        INT, STRING, ERR
    } BaseType;
    
    typedef struct ExprRes {
        ExprRes() {}
        
        ExprRes(BaseType t) {
            type = t;
            str = "";
            n = 0;
        }
        
        ExprRes(const ExprRes &r) {
            type= r.type;
            str = r.str;
            n = r.n;
        }
        
        BaseType type;
        std::string str;
        int n;
    } ExprRes;
        
    ExprRes exprRes_;
    
    static bool err(const ExprRes &e1);
    static bool eq(const ExprRes &e1, const BaseType &t);
    
private:
    Character *ch_;
    
    std::unordered_map<std::string, ExprRes> vals_;
    std::vector<Msl::MoveScript*> scripts_;
    std::unordered_map<std::string, std::function<void(void)>> bindings_;
};

#endif /* MslInterpreter_hpp */
