/* 
 * File:   segR.h
 * Author: luchinsky
 *
 * Created on January 18, 2020, 11:23 AM
 */

#ifndef SEGR_H
#define SEGR_H

#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#include "EvtGen/EvtGen.hh"
#include "EvtGenBase/EvtPatches.hh"
#include "EvtGenBase/EvtParticleFactory.hh"
#include "EvtGenBase/EvtStdHep.hh"
#include "EvtGenBase/EvtParticle.hh"
#include "EvtGenBase/EvtRandom.hh"
#include "EvtGenBase/EvtRandomEngine.hh"
#include "EvtGenBase/EvtReport.hh"
#include "EvtGenModels/EvtModelReg.hh"

/// use of external generators
#if EVTGEN_EXTERNAL
#include "EvtGenExternal/EvtExternalGenList.hh"
#include "EvtGenBase/EvtAbsRadCorr.hh"
#include "EvtGenBase/EvtDecayBase.hh"
#endif

#include "TFile.h"
#include "TTree.h"
#include "TStopwatch.h"
#include "TRandom3.h"

#define MAX 200

#ifndef EVTGEN_EVT_INFO
#define EVTGEN_EVT_INFO EVT_INFO
#endif

class EvtRootRandomEngine : public EvtRandomEngine {
public:

    EvtRootRandomEngine(int s = 0) {
        seed = s;
    }
    double random();
    int seed;
};

//Define random number fcn used by Jetset
extern "C" {
    extern float rlu_();
    extern float begran_(int *);
}

void segR(int argc, char *argv[]);


#endif /* SEGR_H */

