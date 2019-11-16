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

RrfVar *varFactory(string str) {
    if (str.substr(0, 2) == "E_" || str.substr(0, 2) == "e_") {
        return new RrfVarE(str);
    } else if (str.substr(0, 3) == "px_" || str.substr(0, 3) == "pX_" || str.substr(0, 3) == "Px_") {
        return new RrfVarPx(str);
    } else if (str.substr(0, 3) == "py_" || str.substr(0, 3) == "pY_" || str.substr(0, 3) == "Py_") {
        return new RrfVarPy(str);
    } else if (str.substr(0, 3) == "pz_" || str.substr(0, 3) == "pZ_" || str.substr(0, 3) == "Pz_") {
        return new RrfVarPz(str);
    } else {
        cout << " varFactory: Unknown variable " << str << "!" << endl;
        ::abort();
    }
}

float RrfVarE::getValue(RrfEvent* event) {
    return event->get_mom_from_arg(var, 2, var.length()).get(0);
};

float RrfVarPx::getValue(RrfEvent* event) {
    return event->get_mom_from_arg(var, 3, var.length()).get(1);
};

float RrfVarPy::getValue(RrfEvent* event) {
    return event->get_mom_from_arg(var, 3, var.length()).get(2);
}

float RrfVarPz::getValue(RrfEvent* event) {
    return event->get_mom_from_arg(var, 3, var.length()).get(3);
}