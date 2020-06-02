//
//  MslScanner.cpp
//  SFML-App
//
//  Created by Jack Erb on 5/30/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "MslScanner.hpp"

#include <iostream>
#include <string>
#include <regex>
#include <sstream>

using std::cerr;
using std::endl;
using std::string;

void MslScanner::FeedInput(std::string filename) {
    file_ = std::ifstream(filename);
    if (!file_.is_open()) {
        cerr << "Error opening file: " << filename <<endl;
    }
}

void MslScanner::nextLine() {
    // Get the next line from the file
    if (!std::getline(file_, line_)) {
        eof_ = true;
        file_.close();
    }
    
    // Prep the string
    std::regex r("[+-*/(){}=;:#]");
    line_ = std::regex_replace(line_, r, " $0 ");
}

Msl::Token MslScanner::NextToken() {
    if (!eof_ && line_.size() == 0) nextLine();
    if (eof_) return Msl::EOF_;

    static char whitespace[] = " \n\t";
    
    string::size_type p1, p2;
    while (true) {
        p1 = line_.find_first_not_of(whitespace);
        if (p1 == string::npos) {
            // Empty line
            nextLine();
            if (eof_) return Msl::EOF_;
            continue;
        } else if (line_[p1] == '#') {
            // Comment; ignore line
            nextLine();
            if (eof_) return Msl::EOF_;
            continue;
        }
        
        p2 = line_.find_first_of(whitespace, p1);
        if (p2 == string::npos) {
            Msl::Token token = getToken(line_.substr(p1));
            line_ = "";
            cerr << "TOKEN " << Msl::toString(token) << endl;
            return token;
        }
        
        // Next token is substr of p1 to p2
        Msl::Token token = getToken(line_.substr(p1, (p2 - p1)));
        // Clip line
        line_ = line_.substr(p2);
        return token;
    }
    return Msl::EOF_;
}

Msl::Token MslScanner::getToken(std::string s) {
    // Terrible token matcher...
    if (s.size() == 1) {
        if (s.compare("{") == 0) return Msl::LCURL;
        if (s.compare("}") == 0) return Msl::RCURL;
        if (s.compare("(") == 0) return Msl::LPAREN;
        if (s.compare(")") == 0) return Msl::RPAREN;
        if (s.compare("=") == 0) return Msl::EQUALS;
        if (s.compare("+") == 0) return Msl::PLUS;
        if (s.compare("-") == 0) return Msl::MINUS;
        if (s.compare("*") == 0) return Msl::TIMES;
        if (s.compare("/") == 0) return Msl::DIV;
        if (s.compare("=") == 0) return Msl::EQUALS;
        if (s.compare("+") == 0) return Msl::PLUS;
        if (s.compare("-") == 0) return Msl::MINUS;
        if (s.compare("*") == 0) return Msl::TIMES;
        if (s.compare("/") == 0) return Msl::DIV;
        if (s.compare(";") == 0) return Msl::EOL;
        if (s.compare(":") == 0) return Msl::COLON;
    } else {
        if (s.compare("func") == 0) return Msl::FUNC;
        if (s.compare("var") == 0) return Msl::VAR;
        if (s.compare("switch") == 0) return Msl::SWITCH;
        if (s.compare("case") == 0) return Msl::CASE;
        if (s.compare("default") == 0) return Msl::DEFAULT;
    }
    
    // Check if int literal
    int n;
    std::istringstream(s) >> n;
    if (n != 0 || s.compare("0") == 0) {
        n_ = n;
        return Msl::INT;
    }
    
    // Assume identifier
    // TODO: TEST IF VALID CHARACTERS
    str_ = s;
    return Msl::IDENTIFIER;
}

std::string MslScanner::getId() {
    return str_;
}

std::string MslScanner::getStringLiteral() {
    return str_;
}

int MslScanner::getIntLiteral() {
    return n_;
}
