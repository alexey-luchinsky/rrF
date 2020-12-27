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
        if(i>=nTrk) {
            cout << " Particle number " << i << " is larger then the number of tracks "<< nTrk << endl;
            return p;
        };
        p.set(fE[i], fPx[i], fPy[i], fPz[i]);
        return p;
    } else {
        if(i>=desc->size()) {
            cout << " Particle number " << i << " is larger then the size of the descriptor " << desc->size() << endl;
            return p;
        }
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
