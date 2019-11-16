/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RrfVar.h
 * Author: luchinsky
 *
 * Created on November 16, 2019, 6:48 AM
 */

#ifndef RRFVAR_H
#define RRFVAR_H


#include "RrfEvent.h"
using namespace std;

class RrfVar {
public:
    RrfVar() {};
    RrfVar(const RrfVar& orig) {};
    virtual ~RrfVar() {};
    virtual float getValue(RrfEvent *event) = 0;
    virtual string to_string() = 0;
};

RrfVar *varFactory(string str);

class RrfVarE: public RrfVar {
public:
    RrfVarE(string _str);
    string to_string();
    float getValue(RrfEvent *event);
private:
    string var;
};

class RrfVarPx: public RrfVar {
public:
    RrfVarPx(string _str);
    string to_string();
    float getValue(RrfEvent *event);
private:
    string var;
};


#endif /* RRFVAR_H */

