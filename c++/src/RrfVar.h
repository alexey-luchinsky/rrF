/*******************************************************************************
* Copyright 2019 Alexey Luchinsky                                              *
*   email: alexey.luchinsky@gmail.com                                          *
*                                                                              *
* This program is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU General Public License as published by         *
* the Free Software Foundation, either version 3 of the License, or            *
* (at your option) any later version.                                          *
*                                                                              *
* It is distributed in the hope that it will be useful,                        *
* but WITHOUT ANY WARRANTY; without even the implied warranty of               *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                *
* GNU General Public License for more details.                                 *
*                                                                              *
* You should have received a copy of the GNU General Public License            *
* along with this program.  If not, see   <https://www.gnu.org/licenses/>.     *
*******************************************************************************/


#ifndef RRFVAR_H
#define RRFVAR_H


#include "RrfEvent.h"
#include "EvtGenBase/EvtKine.hh"
using namespace std;

class RrfVar {
public:
    RrfVar(string str, std::vector<int> *desc)  { var = str; descriptor=desc;}
    RrfVar(const RrfVar& orig) {};
    virtual ~RrfVar() {};
    virtual float getValue(RrfEvent *event) = 0;
    string to_string() { return var;};
protected:
    string var;
    double cos_between(EvtVector4R p1, EvtVector4R p2);
    std::vector<int> *descriptor;
};

RrfVar *varFactory(string str, std::vector<int> *descriptor);

class RrfVarE: public RrfVar {
public:
    RrfVarE(string str, std::vector<int> *d): RrfVar(str, d){};
    float getValue(RrfEvent *event);
};

class RrfVarPx: public RrfVar {
public:
    RrfVarPx(string str, std::vector<int> *d): RrfVar(str, d) {};
    float getValue(RrfEvent *event);
};

class RrfVarPy: public RrfVar {
public:
    RrfVarPy(string str, std::vector<int> *d): RrfVar(str, d) {};
    float getValue(RrfEvent *event);
};

class RrfVarPz: public RrfVar {
public:
    RrfVarPz(string str, std::vector<int> *d): RrfVar(str, d) {};
    float getValue(RrfEvent *event);
};

class RrfVarPT: public RrfVar {
public:
    RrfVarPT(string str, std::vector<int> *d): RrfVar(str, d) {};
    float getValue(RrfEvent *event);
protected:
    EvtVector4R P;
};

class RrfVarM2: public RrfVar {
public:
    RrfVarM2(string str, std::vector<int> *d): RrfVar(str, d) {};
    float getValue(RrfEvent *event);
};

class RrfVarM: public RrfVar {
public:
    RrfVarM(string str, std::vector<int> *d): RrfVar(str, d) {};
    float getValue(RrfEvent *event);
};

class RrfVarId: public RrfVar {
public:
    RrfVarId(string str, std::vector<int> *d): RrfVar(str, d) {};
    float getValue(RrfEvent *event);
};


class RrfVarCos: public RrfVar {
public:
    RrfVarCos(string str, std::vector<int> *d): RrfVar(str, d) {};
    float getValue(RrfEvent *event);
};

class RrfVarCos0: public RrfVar {
public:
    RrfVarCos0(string str, std::vector<int> *d): RrfVar(str, d) {};
    float getValue(RrfEvent *event);
};

class RrfVarCth: public RrfVar {
public:
    RrfVarCth(string str, std::vector<int> *d): RrfVar(str, d) {};
    float getValue(RrfEvent *event);
};

class RrfVarProb: public RrfVar {
public:
    RrfVarProb(string str, std::vector<int> *d): RrfVar(str, d) {};
    float getValue(RrfEvent *event);
};

class RrfVarNTracks: public RrfVar {
public:
    RrfVarNTracks(string str, std::vector<int> *d): RrfVar(str, d) {};
    float getValue(RrfEvent *event);
};


#endif /* RRFVAR_H */

