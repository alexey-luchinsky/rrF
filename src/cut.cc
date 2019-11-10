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

#include "cut.h"

cut::cut(std::string s) {
    var = "e_1";
    operation = '>';
    value = 0.5;
}

cut::cut(const cut& orig) {
}

cut::~cut() {
}

bool cut::is_ok() {
    float v = calc_var(var);
    if(operation == '>') {
        return v > value;
    }
    else if(operation == '<') {
        return v < value;
    }
    else {
        return true;
    }
}

