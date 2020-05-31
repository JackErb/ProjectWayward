//
//  MslParser.cpp
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>

#include "MslParser.hpp"
#include "MslScanner.hpp"
#include "Msl.hpp"
#include "Func.h"
#include "Expression.h"
#include "AssignStatement.h"
#include "FunctionCall.h"
#include "Plus.h"
#include "Minus.h"
#include "Times.h"
#include "Divide.h"
#include "Var.h"
#include "IntLiteral.h"
#include "StringLiteral.h"

using std::string;
using std::unordered_map;
using std::vector;
using std::cerr;
using std::endl;

void MslParser::error(std::string str) {
    cerr << str << endl;
    err_ = true;
}

bool MslParser::assertToken(Msl::Token exp, Msl::Token act) {
    if (exp != act) {
        error("Expected " + Msl::toString(exp) + " but got " + Msl::toString(act));
        return false;
    }
    return true;
}

bool MslParser::assertNext(Msl::Token exp) {
    curr_ = s_->NextToken();
    return assertToken(exp, curr_);
}

MoveLogic* MslParser::parseProgram(MslScanner *s) {
    MoveLogic* res = new unordered_map<string, Func*>();
    s_ = s;
    
    curr_ = s->NextToken();
    while (curr_ != Msl::EOF_) {
        switch (curr_) {
            case Msl::FUNC: {
                // func <ID>() {}
                assertNext(Msl::IDENTIFIER);
                string name = s_->getId();
                
                if (res->find(name) != res->end()) {
                    error("duplicate function declaration " + name);
                    break;
                }
                
                assertNext(Msl::LPAREN);
                assertNext(Msl::RPAREN);
                assertNext(Msl::LCURL);
                
                curr_ = s_->NextToken();
                vector<Statement*> ls;
                while (curr_ != Msl::RCURL) {
                    Statement *s = statement();
                    if (err_) break;
                    ls.push_back(s);
                }
                
                (*res)[name] = new Func(name, ls);
                break;
            }
            default: {
                error("unexpected token " + Msl::toString(curr_));
                break;
            }
        }
        
        if (err_) {
            cerr << "Error encountered, quitting parsing" << endl;
            return nullptr;
        }
        
        curr_ = s_->NextToken();
    }
    
    return res;
}

Statement* MslParser::statement() {
    switch (curr_) {
        case Msl::SWITCH: {
            // Switch statement
            error("Switch not implemented");
            return nullptr;
        }
        case Msl::VAR: {
            assertNext(Msl::IDENTIFIER);
            string name = s_->getId();
            assertNext(Msl::EQUALS);
            
            curr_ = s_->NextToken();
            Expression *e = expr();
            return new AssignStatement(name, e);
        }
        case Msl::IDENTIFIER: {
            // Function call
            string name = s_->getId();
            assertNext(Msl::LPAREN);
            error("Function call not implemented");
        }
        default: {
            error("unexpected token " + Msl::toString(curr_));
            return nullptr;
        }
    }
}

Expression* MslParser::expr() {
    Expression *e1 = term();
    if (err_) return nullptr;
    
    return exprtail(e1);
}

Expression* MslParser::exprtail(Expression *e1) {
    switch (curr_) {
        case Msl::PLUS: {
            curr_ = s_->NextToken();
            Expression *e2 = term();
            if (err_) return nullptr;
            
            return exprtail(new Plus(e1, e2));
        }
        case Msl::MINUS: {
            curr_ = s_->NextToken();
            Expression *e2 = term();
            if (err_) return nullptr;
            
            return exprtail(new Minus(e1, e2));
        }
        case Msl::RPAREN:
        case Msl::EOL:
            curr_ = s_->NextToken();
            return e1;
        default:
            error("expr(): unexpected token " + Msl::toString(curr_));
            return nullptr;
    }
}

Expression *MslParser::term() {
    Expression *e1 = factor();
    if (err_) return nullptr;
    curr_ = s_->NextToken();
    
    return termtail(e1);
}

Expression *MslParser::termtail(Expression *e1) {
    switch (curr_) {
        case Msl::TIMES: {
            curr_ = s_->NextToken();
            Expression *e2 = factor();
            if (err_) return nullptr;
            
            curr_ = s_->NextToken();
            return termtail(new Times(e1, e2));
        }
        case Msl::DIV: {
            curr_ = s_->NextToken();
            Expression *e2 = factor();
            if (err_) return nullptr;
            
            curr_ = s_->NextToken();
            return termtail(new Divide(e1, e2));
        }
        case Msl::PLUS:
        case Msl::MINUS:
        case Msl::RPAREN:
        case Msl::EOL:
            return e1;
        default:
            error("term(): unexpected token " + Msl::toString(curr_));
            return nullptr;
    }
}

Expression *MslParser::factor() {
    switch (curr_) {
        case Msl::IDENTIFIER:
            return new Var(s_->getId());
        case Msl::INT:
            return new IntLiteral(s_->getIntLiteral());
        case Msl::LPAREN:
            return expr();
        default:
            error("factor(): unexpected token " + Msl::toString(curr_));
            return nullptr;
    }
}
