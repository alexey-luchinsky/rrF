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
    RrfVar(string str)  { var = str;};
    RrfVar(const RrfVar& orig) {};
    virtual ~RrfVar() {};
    virtual float getValue(RrfEvent *event) = 0;
    string to_string() { return var;};
private:
    string var;
};

RrfVar *varFactory(string str);

class RrfVarE: public RrfVar {
public:
    RrfVarE(string str): RrfVar(str)  { var = str;};
    float getValue(RrfEvent *event);
private:
    string var;
};

class RrfVarPx: public RrfVar {
public:
    RrfVarPx(string str): RrfVar(str)  { var = str;};
    float getValue(RrfEvent *event);
private:
    string var;
};


#endif /* RRFVAR_H */

