/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RrfEvent.h
 * Author: luchinsky
 *
 * Created on November 15, 2019, 6:38 AM
 * 
 * RrfEvent is the class that contains all information about the current event
 */

#ifndef RRFEVENT_H
#define RRFEVENT_H

#include "TTree.h"
#include "EvtGenBase/EvtVector4R.hh"


#define MAX 100

class RrfEvent {
public:
    RrfEvent();
    RrfEvent(const RrfEvent& orig);
    virtual ~RrfEvent();
    void init_input_fields(TTree *ntp);
    int char_to_ind(char c);
    EvtVector4R get_mom_from_arg(std::string var, int start_pos, int end_pos);

    Int_t ev = 0;
    Int_t nLine[MAX], pdgID[MAX], nDau[MAX], nM1[MAX], nM2[MAX], nDF[MAX], nDL[MAX];
    Double_t fPx[MAX], fPy[MAX], fPz[MAX], fE[MAX];
    Double_t fVx[MAX], fVy[MAX], fVz[MAX], fT[MAX];
    Double_t fTht[MAX], fM[MAX], fP[MAX], fPt[MAX];
    Int_t nTrk = 0;
    Double_t fProb;

private:

};

#endif /* RRFEVENT_H */

