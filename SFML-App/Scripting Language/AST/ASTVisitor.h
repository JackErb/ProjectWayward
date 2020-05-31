//
//  ASTVisitor.h
//  SFML-App
//
//  Created by Jack Erb on 5/31/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef ASTVisitor_h
#define ASTVisitor_h

class AssignStatement;
class SwitchStatement;
class Block;
class FunctionCall;
class Plus;
class Minus;
class Times;
class Divide;
class IntLiteral;
class StringLiteral;
class Var;

class Visitor {
public:
    virtual void visit(AssignStatement *s) = 0;
    virtual void visit(SwitchStatement *s) = 0;
    virtual void visit(Block *s) = 0;
    virtual void visit(FunctionCall *s) = 0;
    
    virtual void visit(Plus *e) = 0;
    virtual void visit(Minus *e) = 0;
    virtual void visit(Var *e) = 0;
    virtual void visit(IntLiteral *e) = 0;
    virtual void visit(StringLiteral *e) = 0;
    virtual void visit(Times *e) = 0;
    virtual void visit(Divide *e) = 0;
};

#endif /* ASTVisitor_h */
