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

class segR {
public:
    segR(int argc, char *argv[]);
    segR(const segR& orig);
    virtual ~segR();
    void run();
private:
    EvtStdHep evtstdhep;
    EvtParticle *parent;
    int seed = -1;
    EvtGen *myGenerator;
    EvtRandomEngine* myRandomEngine = 0;
    EvtId PART;
    int number = 10;
    double val = -3.0969;
    double P = 0.0;
    double E = 0.0;
    double mp = 0.938272;
    double md = 1.875613;
    double mtarg;

    Int_t ev = 0;
    Int_t nLine[MAX], pdgID[MAX], nDau[MAX], nM1[MAX], nM2[MAX], nDF[MAX], nDL[MAX];
    Double_t fPx[MAX], fPy[MAX], fPz[MAX], fE[MAX];
    Double_t fVx[MAX], fVy[MAX], fVz[MAX], fT[MAX];
    Double_t fTht[MAX], fM[MAX], fP[MAX], fPt[MAX];
    Double_t prob;

    Int_t nTrk = 0;
    TTree *ntp;

};

#endif /* SEGR_H */

