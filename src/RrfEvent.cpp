/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RrfEvent.cpp
 * Author: luchinsky
 * 
 * Created on November 15, 2019, 6:38 AM
 */

#include "RrfEvent.h"

using namespace std;

RrfEvent::RrfEvent() {
}

RrfEvent::RrfEvent(const RrfEvent& orig) {
}

RrfEvent::~RrfEvent() {
}

void RrfEvent::init_input_fields(TTree *ntp) {
    cout << "[ntp]=" << ntp->GetEntries() << endl;

    ntp->SetBranchAddress("ev", &ev);
    ntp->SetBranchAddress("nTrk", &nTrk);

    ntp->SetBranchAddress("N", nLine);
    ntp->SetBranchAddress("Id", pdgID);

    ntp->SetBranchAddress("M1", nM1);
    ntp->SetBranchAddress("M2", nM2);
    ntp->SetBranchAddress("DF", nDF);
    ntp->SetBranchAddress("DL", nDL);
    ntp->SetBranchAddress("nDau", nDau);

    ntp->SetBranchAddress("px", fPx);
    ntp->SetBranchAddress("py", fPy);
    ntp->SetBranchAddress("pz", fPz);
    ntp->SetBranchAddress("E", fE);

    ntp->SetBranchAddress("t", fT);
    ntp->SetBranchAddress("x", fVx);
    ntp->SetBranchAddress("y", fVy);
    ntp->SetBranchAddress("z", fVz);

    ntp->SetBranchAddress("m", fM);
    ntp->SetBranchAddress("p", fP);
    ntp->SetBranchAddress("pt", fPt);
    ntp->SetBranchAddress("tht", fTht);
    ntp->SetBranchAddress("prob", &fProb);
}

int RrfEvent::char_to_ind(char c) {
    int ind = c - '0';
    if (ind < 0 || ind >= nTrk) {
        cout << "wrong particle number " << ind << " with nTrk = " << nTrk << endl;
        ::abort();
    };
    return ind;
}

EvtVector4R RrfEvent::get_mom_from_i(int i, std::vector<int> *desc) {
    EvtVector4R p;
    if (desc->size() == 0) {
        p.set(fE[i], fPx[i], fPy[i], fPz[i]);
        return p;
    } else {
        int id = (*desc)[i];
        for (int _i = 0; _i < nTrk; ++_i) {
            if (pdgID[_i] == id) {
                p.set(fE[_i], fPx[_i], fPy[_i], fPz[_i]);
                return p;
            };
        };
        cout << "The particle with id = " << id << " was not found!" << endl;
        return p;
    };
}

EvtVector4R RrfEvent::get_mom_from_arg(string var, int start_pos, int end_pos, std::vector<int> *desc) {
    EvtVector4R P, _p;
    for (int i = start_pos; i < end_pos; ++i) {
        float fact = 1;
        if (var[i] == 'm' && i < var.length()) {
            fact = -1;
            ++i;
        };
        int ind = char_to_ind(var[i]);
        P += fact * get_mom_from_i(ind, desc);
    };
    return P;
}
