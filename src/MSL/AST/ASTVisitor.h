//
//  ASTVisitor.h
//  SFML-App
//
//  Created by Jack Erb on 5/31/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef ASTVisitor_h
#define ASTVisitor_h

struct AssignStatement;
struct SwitchStatement;
struct Block;
struct FunctionCall;
struct Plus;
struct Minus;
struct Times;
struct Divide;
struct IntLiteral;
struct StringLiteral;
struct FloatLiteral;
struct Var;
struct Negate;

class Visitor {
public:
    virtual void visit(Func *s) = 0;
    
    virtual void visit(AssignStatement *s) = 0;
    virtual void visit(SwitchStatement *s) = 0;
    virtual void visit(Block *s) = 0;
    virtual void visit(FunctionCall *s) = 0;
    
    virtual void visit(Plus *e) = 0;
    virtual void visit(Minus *e) = 0;
    virtual void visit(Var *e) = 0;
    virtual void visit(IntLiteral *e) = 0;
    virtual void visit(StringLiteral *e) = 0;
    virtual void visit(FloatLiteral *e) = 0;
    virtual void visit(Times *e) = 0;
    virtual void visit(Divide *e) = 0;
    virtual void visit(Negate *e) = 0;
};

#endif /* ASTVisitor_h */
