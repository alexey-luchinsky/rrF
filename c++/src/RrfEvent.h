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
    EvtVector4R get_mom_from_i(int i, std::vector<int> *desc);
    EvtVector4R get_mom_from_arg(std::string var, int start_pos, int end_pos, std::vector<int> *desc);

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

