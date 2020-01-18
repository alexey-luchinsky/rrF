/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   segR.cc
 * Author: luchinsky
 * 
 * Created on January 18, 2020, 11:23 AM
 */

#include "segR.h"

double EvtRootRandomEngine::random() {
    static TRandom3 randengine(seed);
    return randengine.Rndm();
}


segR::segR() {
}

segR::segR(const segR& orig) {
}

segR::~segR() {
}

