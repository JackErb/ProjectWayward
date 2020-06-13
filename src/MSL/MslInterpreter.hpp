//
//  MslInterpreter.hpp
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef MslInterpreter_hpp
#define MslInterpreter_hpp

#include "../Game/Loaders/MoveLoader.hpp"
#include "Msl.hpp"
#include "AST/ASTVisitor.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <functional>
#include <chrono>

class Character;

using namespace std::chrono;

class MslInterpreter: public Visitor {
public:
    MslInterpreter();
    virtual ~MslInterpreter();

    void Init(std::vector<std::string> scripts);
    void InitScript(std::string move);
    
    void PreTick(int frame);
    void ProcessInput();
    void Tick();
    void CallFunction(std::string name);
    
    // Parameter accessing
    int numParams() { return params_.size(); }
    int getIntParam(int n) { return params_[n].n; }
    std::string getStrParam(int n) { return params_[n].str; }
    float getFloatParam(int n) { return params_[n].f; }
    
    void setVal(std::string v, int n) {
        vals_[v] = ExprRes(n);
    }
    
    // AST Visitor functions
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
    void visit(FloatLiteral *e);
    void visit(Times *e);
    void visit(Divide *e);
    void visit(Negate *e);
    
private:
    // Variables used while visiting AST
    typedef enum Type {
        INT, FLOAT, STRING, ERR
    } BaseType;
    
    typedef struct ExprRes {
        ExprRes() {}
        
        ExprRes(int n) : type(INT), n(n) {}
        ExprRes(float f) : type(FLOAT), f(f) {}
        ExprRes(std::string s) : type(STRING), str(s) {}
                
        ExprRes(const ExprRes &r) {
            type= r.type;
            str = r.str;
            n = r.n;
            f = r.f;
        }
        
        BaseType type;
        std::string str;
        int n;
        float f;
    } ExprRes;
        
    ExprRes exprRes_;
    
    static bool err(const ExprRes &e1);
    static bool eq(const ExprRes &e1, const BaseType &t);
    
    void initializeCharacterBindings(Character *ch);
    
public:
    const static std::unordered_map<std::string, Msl::MoveScript*> scripts;
    
    std::unordered_map<std::string, std::function<void(void)> > bindings_;
    // The current running script
    std::string script;
    
private:
    std::unordered_map<std::string, ExprRes> vals_;
    std::vector<ExprRes> params_;
    time_point<high_resolution_clock> time;
};

#endif /* MslInterpreter_hpp */
