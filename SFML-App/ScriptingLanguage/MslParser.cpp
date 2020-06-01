//
//  MslParser.cpp
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

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
#include "Block.h"
#include "Var.h"
#include "IntLiteral.h"
#include "StringLiteral.h"
#include "SwitchStatement.h"
#include "ASTPrintVisitor.h"

#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>

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

Msl::MoveScript* MslParser::parseProgram(MslScanner *s) {
    unordered_map<string, Func*>* res = new unordered_map<string, Func*>();
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
                
                std::pair<string, Func*> p(name, new Func(name, ls));
                res->insert(p);
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
    
    for (auto it = res->begin(); it != res->end(); it++) {
        ASTPrintVisitor pv;
        pv.visit(it->second);
    }
    
    return res;
}

Statement* MslParser::statement() {
    switch (curr_) {
        case Msl::SWITCH: {
            // Switch statement
            // switch <EXPR> {
            // case 0:
            //    ...
            // case 1:
            //    ...
            // }
            curr_ = s_->NextToken();
            statementExpr_ = true;
            SwitchStatement *res = new SwitchStatement(expr());
            statementExpr_ = false;
            if (err_ || !assertToken(curr_, Msl::LCURL)) return nullptr;
            curr_ = s_->NextToken();
            while (true) {
                if (curr_ == Msl::RCURL) {
                    // End switch statement
                    break;
                } else if (curr_ == Msl::CASE || curr_ == Msl::DEFAULT) {
                    int n;
                    Msl::Token c = curr_;
                    if (c == Msl::CASE) {
                        assertNext(Msl::INT);
                        if (err_) return nullptr;
                        n = s_->getIntLiteral();
                    }
                    
                    assertNext(Msl::COLON);
                    if (err_) return nullptr;
                    
                    curr_ = s_->NextToken();
                    Block *b = new Block();
                    while (curr_ != Msl::DEFAULT && curr_ != Msl::CASE && curr_ != Msl::RCURL) {
                        Statement *s = statement();
                        if (err_) return nullptr;
                        b->ls.push_back(s);
                    }
                                       
                    if (c == Msl::CASE) {
                        res->cases[n] = b;
                    } else {
                        res->defBranch = true;
                        res->def = b;
                    }
                } else {
                    error("unexpected token in switch statement: " + Msl::toString(curr_));
                    return nullptr;
                }
            }
            
            curr_ = s_->NextToken();
            return res;
        }
        case Msl::VAR: {
            assertNext(Msl::IDENTIFIER);
            if (err_) return nullptr;
            string name = s_->getId();
            
            assertNext(Msl::EQUALS);
            if (err_) return nullptr;
            
            curr_ = s_->NextToken();
            Expression *e = expr();
            if (err_) return nullptr;
            
            return new AssignStatement(name, e);
        }
        case Msl::IDENTIFIER: {
            // Function call
            string name = s_->getId();
            curr_ = s_->NextToken();
            if (curr_ == Msl::LPAREN) {
                assertNext(Msl::RPAREN);
                if (err_) return nullptr;
                assertNext(Msl::EOL);
                if (err_) return nullptr;
                curr_ = s_->NextToken();
                
                return new FunctionCall(name);
            } else if (curr_ == Msl::EQUALS) {
                curr_ = s_->NextToken();
                Expression *e = expr();
                if (err_) return nullptr;
                
                return new AssignStatement(name, e);
            } else {
                error("statement id: unexpected token " + Msl::toString(curr_));
                return nullptr;
            }
        }
        default: {
            error("statement(): unexpected token " + Msl::toString(curr_));
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
            return e1;
        case Msl::EOL:
            curr_ = s_->NextToken();
            return e1;
        default:
            if (statementExpr_ && curr_ == Msl::LCURL) return e1;
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
            if (statementExpr_ && curr_ == Msl::LCURL) return e1;
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
            curr_ = s_->NextToken();
            return expr();
        default:
            error("factor(): unexpected token " + Msl::toString(curr_));
            return nullptr;
    }
}
