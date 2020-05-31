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
    std::regex r("[+-*/(){}=;]");
    line_ = std::regex_replace(line_, r, " $0 ");
}

Msl::Token MslScanner::NextToken() {
    if (!eof_ && line_.size() == 0) nextLine();
    if (eof_) return Msl::EOF_;

    static char whitespace[] = " \n\t";
    
    string::size_type p1, p2;
    do {
        p1 = line_.find_first_not_of(whitespace);
        if (p1 == string::npos) {
            nextLine();
            if (eof_) return Msl::EOF_;
            continue;
        }
        p2 = line_.find_first_of(whitespace, p1);
        if (p2 == string::npos) {
            Msl::Token token = getToken(line_.substr(p1));
            line_ = "";
            cerr << Msl::toString(token) << " " << endl;
            return token;
        }
        Msl::Token token = getToken(line_.substr(p1, (p2 - p1)));
        line_ = line_.substr(p2);
        cerr << Msl::toString(token) << " ";
        return token;
    } while (p1 == string::npos);
    return Msl::EOF_;
}

Msl::Token MslScanner::getToken(std::string s) {
    if (s.compare("func") == 0) return Msl::FUNC;
    if (s.compare("{") == 0) return Msl::LCURL;
    if (s.compare("}") == 0) return Msl::RCURL;
    if (s.compare("(") == 0) return Msl::LPAREN;
    if (s.compare(")") == 0) return Msl::RPAREN;
    if (s.compare("var") == 0) return Msl::VAR;
    if (s.compare("=") == 0) return Msl::EQUALS;
    if (s.compare("+") == 0) return Msl::PLUS;
    if (s.compare("-") == 0) return Msl::MINUS;
    if (s.compare("*") == 0) return Msl::TIMES;
    if (s.compare("/") == 0) return Msl::DIV;
    if (s.compare("switch") == 0) return Msl::SWITCH;
    if (s.compare(";") == 0) return Msl::EOL;
    
    int n;
    std::istringstream(s) >> n;
    if (n != 0 || s.compare("0") == 0) {
        n_ = n;
        return Msl::INT;
    }
    
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
