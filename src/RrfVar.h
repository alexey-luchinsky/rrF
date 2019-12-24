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
protected:
    string var;
    double cos_between(EvtVector4R p1, EvtVector4R p2);
};

RrfVar *varFactory(string str);

class RrfVarE: public RrfVar {
public:
    RrfVarE(string str): RrfVar(str)  { var = str;};
    float getValue(RrfEvent *event);
};

class RrfVarPx: public RrfVar {
public:
    RrfVarPx(string str): RrfVar(str)  { var = str;};
    float getValue(RrfEvent *event);
};

class RrfVarPy: public RrfVar {
public:
    RrfVarPy(string str): RrfVar(str)  { var = str;};
    float getValue(RrfEvent *event);
};

class RrfVarPz: public RrfVar {
public:
    RrfVarPz(string str): RrfVar(str)  { var = str;};
    float getValue(RrfEvent *event);
};

class RrfVarPT: public RrfVar {
public:
    RrfVarPT(string str): RrfVar(str)  { var = str;};
    float getValue(RrfEvent *event);
protected:
    EvtVector4R P;
};

class RrfVarM2: public RrfVar {
public:
    RrfVarM2(string str): RrfVar(str)  { var = str;};
    float getValue(RrfEvent *event);
};

class RrfVarM: public RrfVar {
public:
    RrfVarM(string str): RrfVar(str)  { var = str;};
    float getValue(RrfEvent *event);
};

class RrfVarId: public RrfVar {
public:
    RrfVarId(string str): RrfVar(str)  { var = str;};
    float getValue(RrfEvent *event);
};


class RrfVarCos: public RrfVar {
public:
    RrfVarCos(string str): RrfVar(str)  { var = str;};
    float getValue(RrfEvent *event);
};


#endif /* RRFVAR_H */

