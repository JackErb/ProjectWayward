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
#include <unordered_map>

class Func;

namespace Msl {
    typedef enum Token {
        FUNC, VAR, EQUALS,
        LPAREN, RPAREN, LCURL, RCURL, COLON, COMMA,
        PLUS, TIMES, MINUS, DIV, SWITCH, CASE,
        IDENTIFIER, INT, FLOAT, STRING, EOF_, EOL,
        INVALID, DEFAULT
    } Token;
    
    typedef enum Funcs {
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
            case CASE:
                return "case";
            case COMMA:
                return ",";
            case LPAREN:
                return "(";
            case RPAREN:
                return ")";
            case LCURL:
                return "{";
            case RCURL:
                return "}";
            case COLON:
                return ":";
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
            case FLOAT:
                return "$$FLOAT";
            case STRING:
                return "$$STRING";
            case EOF_:
                return "$$EOF";
            case EOL:
                return ";";
            case INVALID:
                return "$$INVALID";
            case DEFAULT:
                return "DEFAULT";
        }
    }
    
    typedef enum MoveType {
        NAIR
    } MoveType;

    typedef std::unordered_map<std::string, Func*> MoveScript;
};

#endif /* Msl_hpp */
