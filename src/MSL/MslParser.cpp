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
#include "AST/Func.h"
#include "AST/Expressions/Expression.h"

#include "AST/Expressions/Plus.h"
#include "AST/Expressions/Minus.h"
#include "AST/Expressions/Times.h"
#include "AST/Expressions/Divide.h"
#include "AST/Expressions/IntLiteral.h"
#include "AST/Expressions/StringLiteral.h"
#include "AST/Expressions/Negate.h"
#include "AST/Expressions/Var.h"
#include "AST/Statements/AssignStatement.h"
#include "AST/Statements/FunctionCall.h"
#include "AST/Statements/Block.h"
#include "AST/Statements/SwitchStatement.h"
#include "AST/ASTPrintVisitor.h"

#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <utility>

using std::string;
using std::unordered_map;
using std::vector;
using std::cerr;
using std::endl;
using std::pair;
using std::make_pair;

void MslParser::error(std::string str) {
    cerr << str << endl;
    err_ = true;
}

// Asserts the two tokens are equal
bool MslParser::assertToken(Msl::Token exp, Msl::Token act) {
    if (exp != act) {
        error("Expected " + Msl::toString(exp) + " but got " + Msl::toString(act));
        return false;
    }
    return true;
}

// Set curr_ to next token and assert it's equal to exp
// Return true if equal, else false
bool MslParser::assertNext(Msl::Token exp) {
    curr_ = s_->NextToken();
    return assertToken(exp, curr_);
}

Msl::MoveScript* MslParser::parseProgram(MslScanner *s) {
    s_ = s;
        
    // script "NAME";
    assertNext(Msl::IDENTIFIER);
    if (s->getStringLiteral().compare("script") != 0) {
        cerr << "Script tag header not found, quitting parsing" << endl;
    }
    assertNext(Msl::STRING);
    std::string scriptName = s->getStringLiteral();
    assertNext(Msl::EOL);
    curr_ = s->NextToken();
    
    Msl::MoveScript *res = new Msl::MoveScript();
    res->name = scriptName;
    
    // Keep parsing functions until reached EOF
    // Will exit as soon as an error is encountered
    while (curr_ != Msl::EOF_) {
        switch (curr_) {
            case Msl::FUNC: {
                // func <ID>() { ... }
                assertNext(Msl::IDENTIFIER);
                string name = s_->getId();
                
                if (res->funcs.find(name) != res->funcs.end()) {
                    error("duplicate function declaration " + name);
                    break;
                }
                
                assertNext(Msl::LPAREN);
                assertNext(Msl::RPAREN);
                assertNext(Msl::LCURL);
                
                // Parse the contents of the function
                curr_ = s_->NextToken();
                Block *b = new Block();
                while (curr_ != Msl::RCURL) {
                    Statement *s = statement();
                    if (err_) break;
                    b->ls.push_back(s);
                }
                
                std::pair<string, Func*> p(name, new Func(name, b));
                res->funcs.insert(p);
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
    
    /* Print abstract representation of program */
    /*for (auto it = res->funcs.begin(); it != res->funcs.end(); it++) {
        ASTPrintVisitor pv;
        pv.visit(it->second);
    }*/
    
    return res;
}

Statement* MslParser::statement() {
    switch (curr_) {
        case Msl::SWITCH: {
            // Switch statement
            // switch <EXPR> {
            // case 0: ...
            // case 1: ...
            // default: ...
            // }
            
            // Get switch expression
            curr_ = s_->NextToken();
            statementExpr_ = true;
            SwitchStatement *res = new SwitchStatement(expr());
            statementExpr_ = false;
            if (err_ || !assertToken(curr_, Msl::LCURL)) return nullptr;
            
            // Get case branches
            curr_ = s_->NextToken();
            while (true) {
                if (curr_ == Msl::RCURL) {
                    // End switch statement
                    break;
                } else if (curr_ == Msl::CASE) {
                    // Case branch
                    pair<int, Statement*> r = switchCase();
                    if (err_) return nullptr;
                    res->cases[r.first] = r.second;
                } else if (curr_ == Msl::DEFAULT) {
                    // Default branch
                    pair<int, Statement*> r = switchCase();
                    if (err_) return nullptr;
                    res->defBranch = true;
                    res->def = r.second;
                } else {
                    error("unexpected token in switch statement: " + Msl::toString(curr_));
                    return nullptr;
                }
            }
            
            curr_ = s_->NextToken();
            return res;
        }
        case Msl::VAR: {
            // var <NAME> = <EXPR>
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
                curr_ = s_->NextToken();
                vector<Expression*> params = exprList();
                if (err_) return nullptr;
                
                assertNext(Msl::EOL);
                if (err_) return nullptr;
                
                curr_ = s_->NextToken();
                                
                return new FunctionCall(name, params);
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

pair<int, Statement*> MslParser::switchCase() {
    switch (curr_) {
        case Msl::CASE: {
            // case 0:
            //      Statement;
            //      Statement;
            assertNext(Msl::INT);
            if (err_) return {0, nullptr};
            int n = s_->getIntLiteral();
            
            assertNext(Msl::COLON);
            if (err_) return {0, nullptr};
            
            Block *b = new Block();
            curr_ = s_->NextToken();
            while (curr_ != Msl::DEFAULT && curr_ != Msl::CASE && curr_ != Msl::RCURL) {
                Statement *s = statement();
                if (err_) return {0, nullptr};
                b->ls.push_back(s);
            }
            
            return {n, b};
        }
        case Msl::DEFAULT: {
            // default:
            assertNext(Msl::COLON);
            if (err_) return {0, nullptr};
            
            Block *b = new Block();
            curr_ = s_->NextToken();
            while (curr_ != Msl::DEFAULT && curr_ != Msl::CASE && curr_ != Msl::RCURL) {
                Statement *s = statement();
                if (err_) return {0, nullptr};
                b->ls.push_back(s);
            }
            
            return {0, b};
        }
        default:
            error("unexpected token in switch case " + Msl::toString(curr_));
            return {0, nullptr};
    }
}

vector<Expression*> MslParser::exprList() {
    vector<Expression*> ret;
    exprList_ = true;
    while (curr_ != Msl::RPAREN) {
        Expression *e = expr();
        if (err_) return ret;
        ret.push_back(e);
        if (curr_ == Msl::RPAREN) break;
        
        curr_ = s_->NextToken();
        if (curr_ == Msl::COMMA) {
            curr_ = s_->NextToken();
        }
    }
    exprList_ = false;
    if (curr_ != Msl::RPAREN) {
        error("unexpected token in function call");
        return ret;
    }
    return ret;
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
            parenDepth_--;
            return e1;
        case Msl::EOL:
            curr_ = s_->NextToken();
            return e1;
        default:
            if (statementExpr_ || exprList_) return e1;
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
            if (statementExpr_ || exprList_) return e1;
            error("term(): unexpected token " + Msl::toString(curr_));
            return nullptr;
    }
}

Expression *MslParser::factor() {
    if (curr_ == Msl::MINUS) {
        curr_ = s_->NextToken();
        return new Negate(factor());
    }
    
    switch (curr_) {
        case Msl::IDENTIFIER:
            return new Var(s_->getId());
        case Msl::INT:
            return new IntLiteral(s_->getIntLiteral());
        case Msl::FLOAT:
            return new FloatLiteral(s_->getFloatLiteral());
        case Msl::STRING:
            return new StringLiteral(s_->getStringLiteral());
        case Msl::LPAREN:
            parenDepth_++;
            curr_ = s_->NextToken();
            return expr();
        default:
            error("factor(): unexpected token " + Msl::toString(curr_));
            return nullptr;
    }
}
