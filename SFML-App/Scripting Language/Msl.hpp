//
//  Msl.hpp
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef Msl_hpp
#define Msl_hpp

#include <string>

struct Msl {
public:
    typedef enum Token {
        FUNC, VAR, EQUALS,
        LPAREN, RPAREN, LCURL, RCURL,
        PLUS, TIMES, MINUS, DIV, SWITCH,
        IDENTIFIER, INT, STRING, EOF_, EOL,
        INVALID
    } Token;
    
    typedef enum Func {
        INPUT, TICK, COLLISION
    } MainFuncs;
    
    static std::string toString(Token t) {
        switch (t) {
            case FUNC:
                return "func";
            case VAR:
                return "var";
            case EQUALS:
                return "=";
            case SWITCH:
                return "switch";
            case LPAREN:
                return "(";
            case RPAREN:
                return ")";
            case LCURL:
                return "{";
            case RCURL:
                return "}";
            case PLUS:
                return "+";
            case MINUS:
                return "-";
            case TIMES:
                return "*";
            case DIV:
                return "/";
            case IDENTIFIER:
                return "$$ID";
            case INT:
                return "$$INT";
            case STRING:
                return "$$STRING";
            case EOF_:
                return "$$EOF";
            case EOL:
                return ";";
            case INVALID:
                return "$$INVALID";
        }
    }
};

#endif /* Msl_hpp */
