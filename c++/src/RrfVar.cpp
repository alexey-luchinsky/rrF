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


#include "RrfVar.h"

double RrfVar::cos_between(EvtVector4R p1, EvtVector4R p2) {
    double mag1 = 0, mag2 = 0, p1p2 = 0;
    for (int i = 1; i <= 3; ++i) {
        mag1 += p1.get(i) * p1.get(i);
        mag2 += p2.get(i) * p2.get(i);
        p1p2 += p1.get(i) * p2.get(i);
    }
    return p1p2 / sqrt(mag1) / sqrt(mag2);
}

RrfVar *varFactory(string str, std::vector<int> *descriptor) {
    if (str.substr(0, 2) == "E_" || str.substr(0, 2) == "e_") {
        return new RrfVarE(str, descriptor);
    } else if (str.substr(0, 3) == "px_" || str.substr(0, 3) == "pX_" || str.substr(0, 3) == "Px_") {
        return new RrfVarPx(str, descriptor);
    } else if (str.substr(0, 3) == "py_" || str.substr(0, 3) == "pY_" || str.substr(0, 3) == "Py_") {
        return new RrfVarPy(str, descriptor);
    } else if (str.substr(0, 3) == "pz_" || str.substr(0, 3) == "pZ_" || str.substr(0, 3) == "Pz_") {
        return new RrfVarPz(str, descriptor);
    } else if (str.substr(0, 3) == "pt_" || str.substr(0, 3) == "pT_" || str.substr(0, 3) == "PT_") {
        return new RrfVarPT(str, descriptor);
    } else if (str.substr(0, 3) == "m2_" || str.substr(0, 3) == "M2_") {
        return new RrfVarM2(str, descriptor);
    } else if (str.substr(0, 2) == "m_" || str.substr(0, 2) == "M_") {
        return new RrfVarM(str, descriptor);
    } else if (str.substr(0, 3) == "id_" || str.substr(0, 3) == "Id_" || str.substr(0, 3) == "ID_") {
        return new RrfVarId(str, descriptor);
    } else if (str.substr(0, 4) == "cos_" && str.length() == 7) {
        return new RrfVarCos(str, descriptor);
    } else if (str.substr(0, 5) == "cos0_") {
        return new RrfVarCos0(str, descriptor);
    } else if (str.substr(0, 4) == "cth_") {
        return new RrfVarCth(str, descriptor);
     } else if (str == "prob") {
         return new RrfVarProb(str, descriptor);
   } else if (str == "nt") {
        return new RrfVarNTracks(str, descriptor);
    } else {
            cout << " varFactory: Unknown variable " << str << "!" << endl;
            ::abort();
    }
}

float RrfVarE::getValue(RrfEvent* event) {
    return event->get_mom_from_arg(var, 2, var.length(), descriptor).get(0);
};

float RrfVarPx::getValue(RrfEvent* event) {
    return event->get_mom_from_arg(var, 3, var.length(), descriptor).get(1);
};

float RrfVarPy::getValue(RrfEvent* event) {
    return event->get_mom_from_arg(var, 3, var.length(), descriptor).get(2);
}

float RrfVarPz::getValue(RrfEvent* event) {
    return event->get_mom_from_arg(var, 3, var.length(), descriptor).get(3);
}

float RrfVarPT::getValue(RrfEvent* event) {
    P = event->get_mom_from_arg(var, 3, var.length(), descriptor);
    return sqrt(P.get(1) * P.get(1) + P.get(2) * P.get(2));
}

float RrfVarM2::getValue(RrfEvent* event) {
    return event->get_mom_from_arg(var, 3, var.length(), descriptor).mass2();
}

float RrfVarM::getValue(RrfEvent* event) {
    return event->get_mom_from_arg(var, 2, var.length(), descriptor).mass();
}

float RrfVarId::getValue(RrfEvent* event) {
    int ind = event->char_to_ind(var[3]);
    return event->pdgID[ind];
}

float RrfVarCos::getValue(RrfEvent* event) {
    EvtVector4R p1 = event->get_mom_from_arg(var, 4, 5, descriptor);
    EvtVector4R p2 = event->get_mom_from_arg(var, 6, 7, descriptor);
    return cos_between(p1, p2);
}

float RrfVarCos0::getValue(RrfEvent* event) {
    size_t pos2 = var.find("_", 6);
    EvtVector4R k0 = event->get_mom_from_i(0, descriptor);
    EvtVector4R k1 = event->get_mom_from_arg(var, 5, pos2, descriptor);
    EvtVector4R k2 = event->get_mom_from_arg(var, pos2 + 1, var.length(), descriptor);
    return EvtDecayAngle(k0, k1, k2);
}

float RrfVarCth::getValue(RrfEvent* event) {
    EvtVector4R P = event->get_mom_from_arg(var, 4, var.length(), descriptor);
    return P.get(3) / sqrt(P.get(1) * P.get(1) + P.get(2) * P.get(2) + P.get(3) * P.get(3));
}

float RrfVarProb::getValue(RrfEvent* event) {
        return event->fProb;
}

float RrfVarNTracks::getValue(RrfEvent *event) {
    return event->nTrk;
}