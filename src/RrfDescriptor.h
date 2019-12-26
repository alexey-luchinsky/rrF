/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RrfDescriptor.h
 * Author: luchinsky
 *
 * Created on December 26, 2019, 10:52 AM
 */

#ifndef RRFDESCRIPTOR_H
#define RRFDESCRIPTOR_H

#include <vector>
#include <stdio.h>

#include "EvtGenBase/EvtVector4R.hh"

class MomsVector {
public:
    MomsVector() {};
    int size() { return moms.size();}
    EvtVector4R &operator[](std::size_t idx) { return moms[idx];}
    void add(EvtVector4R p) { moms.push_back(p);};
private:
    std::vector<EvtVector4R> moms;
};


class RrfDescriptor {
public:
    RrfDescriptor();
    RrfDescriptor(const RrfDescriptor& orig);
    virtual ~RrfDescriptor();
private:

};

#endif /* RRFDESCRIPTOR_H */

