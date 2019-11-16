/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RrfVar.cpp
 * Author: luchinsky
 * 
 * Created on November 16, 2019, 6:48 AM
 */

#include "RrfVar.h"

RrfVarE::RrfVarE(string _str) {
    var = _str;
}

string RrfVarE::to_string() {
    return var;
}

float RrfVarE::getValue(RrfEvent* event) {
    return event->get_mom_from_arg(var, 2, var.length()).get(0);
};