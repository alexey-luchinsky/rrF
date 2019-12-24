/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RrfVar.cpp
 * Author: luchinsky
 * 
 * Created on November 16, 2019, 6:48 AM
 */

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

RrfVar *varFactory(string str) {
    if (str.substr(0, 2) == "E_" || str.substr(0, 2) == "e_") {
        return new RrfVarE(str);
    } else if (str.substr(0, 3) == "px_" || str.substr(0, 3) == "pX_" || str.substr(0, 3) == "Px_") {
        return new RrfVarPx(str);
    } else if (str.substr(0, 3) == "py_" || str.substr(0, 3) == "pY_" || str.substr(0, 3) == "Py_") {
        return new RrfVarPy(str);
    } else if (str.substr(0, 3) == "pz_" || str.substr(0, 3) == "pZ_" || str.substr(0, 3) == "Pz_") {
        return new RrfVarPz(str);
    } else if (str.substr(0, 3) == "pt_" || str.substr(0, 3) == "pT_" || str.substr(0, 3) == "PT_") {
        return new RrfVarPT(str);
    } else if (str.substr(0, 3) == "m2_" || str.substr(0, 3) == "M2_") {
        return new RrfVarM2(str);
    } else if (str.substr(0, 2) == "m_" || str.substr(0, 2) == "M_") {
        return new RrfVarM(str);
    } else if (str.substr(0, 3) == "id_" || str.substr(0, 3) == "Id_" || str.substr(0, 3) == "ID_") {
        return new RrfVarId(str);
    } else if (str.substr(0, 4) == "cos_" && str.length() == 7) {
        return new RrfVarCos(str);
    } else if (str.substr(0, 5) == "cos0_") {
        return new RrfVarCos0(str);
    } else {
        cout << " varFactory: Unknown variable " << str << "!" << endl;
        ::abort();
    }
}

float RrfVarE::getValue(RrfEvent* event) {
    return event->get_mom_from_arg(var, 2, var.length()).get(0);
};

float RrfVarPx::getValue(RrfEvent* event) {
    return event->get_mom_from_arg(var, 3, var.length()).get(1);
};

float RrfVarPy::getValue(RrfEvent* event) {
    return event->get_mom_from_arg(var, 3, var.length()).get(2);
}

float RrfVarPz::getValue(RrfEvent* event) {
    return event->get_mom_from_arg(var, 3, var.length()).get(3);
}

float RrfVarPT::getValue(RrfEvent* event) {
    P = event->get_mom_from_arg(var, 3, var.length());
    return sqrt(P.get(1) * P.get(1) + P.get(2) * P.get(2));
}

float RrfVarM2::getValue(RrfEvent* event) {
    return event->get_mom_from_arg(var, 3, var.length()).mass2();
}

float RrfVarM::getValue(RrfEvent* event) {
    return event->get_mom_from_arg(var, 2, var.length()).mass();
}

float RrfVarId::getValue(RrfEvent* event) {
        int ind = event->char_to_ind(var[3]);
        return event->pdgID[ind];
}

float RrfVarCos::getValue(RrfEvent* event) {
        EvtVector4R p1 = event->get_mom_from_arg(var, 4, 5);
        EvtVector4R p2 = event->get_mom_from_arg(var, 6, 7);
        return cos_between(p1, p2);    
}

float RrfVarCos0::getValue(RrfEvent* event) {
        size_t pos2 = var.find("_", 6);
        EvtVector4R k1 = event->get_mom_from_arg(var, 5, pos2);
        EvtVector4R k2 = event->get_mom_from_arg(var, pos2 + 1, var.length());
        EvtVector4R k10 = k1;
        k10.applyBoostTo(k2, true);
        return cos_between(k2, k10);
}