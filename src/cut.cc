/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   cut.cc
 * Author: luchinsky
 * 
 * Created on November 10, 2019, 7:25 AM
 */

#include <vector>

#include "cut.h"

void ReplaceStringInPlace(std::string& subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}



cut::cut(std::string s, std::vector<int> d) {
    string _s = s;
    descriptor_vec = d;
    ReplaceStringInPlace(_s,">"," > ");
    ReplaceStringInPlace(_s,"<"," < ");
    ReplaceStringInPlace(_s,"="," = ");
    ReplaceStringInPlace(_s,"+-"," +- ");
    vector<string> args = split_string(_s, " ");
    if(args.size() != 3 && args.size() !=5) {
        cout<<" WRONG FORMAT of the cut " << s << "!" << endl;
        return;
    };
    var = varFactory(args[0], &descriptor_vec);
    operation = args[1];
    value = atof(args[2].c_str());
    error = 1e-5;
    if(operation == "=" && args[3]=="+-") {
        error = atof(args[4].c_str());
    }
}

cut::cut(const cut& orig) {
}

cut::~cut() {
    delete var;
}

bool cut::is_ok(RrfEvent *event) {
    float v = var->getValue(event);
    if(operation == ">") {
        return v > value;
    }
    else if(operation == "<") {
        return v < value;
    }
    else if(operation == "=") {
        return value - error < v && v < value + error;
    }
    else {
        return true;
    }
}

