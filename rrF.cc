/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "tclap/CmdLine.h"
#include <iostream>
#include <regex>
#include "TFile.h"
#include "TNtuple.h"
#include "TH1F.h"
#include "TLorentzVector.h"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtId.hh"

using namespace TCLAP;
using namespace std;

// command line parameters
string inFileName, outFileName;
vector<string> vars;
int nev;
bool print_ids;

// input file fields
#define MAX 100
Int_t ev = 0;
Int_t nLine[MAX], pdgID[MAX], nDau[MAX], nM1[MAX], nM2[MAX], nDF[MAX], nDL[MAX];
Double_t fPx[MAX], fPy[MAX], fPz[MAX], fE[MAX];
Double_t fVx[MAX], fVy[MAX], fVz[MAX], fT[MAX];
Double_t fTht[MAX], fM[MAX], fP[MAX], fPt[MAX];
Int_t nTrk = 0;
Double_t fProb;

void read_args(int argc, char **argv) {
    try {
        TCLAP::CmdLine cmd("Reads ROOT file", ' ', "0.1");
        ValueArg<string> inFileName_arg("i", "in", "input ROOT file", false, "evtOutput.root", "string", cmd);
        ValueArg<string> outFileName_arg("o", "out", "output ROOT file", false, "out.root", "string", cmd);
        MultiArg<string> vars_arg("v", "var", "variable to be saved, e.g. m2_12", true, "string", cmd);
        ValueArg<float> nev_arg("n", "nev", "Number of events to be read (negative if all events should be read)", false, -1, "float", cmd);
        SwitchArg print_ids_arg("p", "print-ids", "should we print ids of the particles", false);
        cmd.add(print_ids_arg);

        cmd.parse(argc, argv);
        inFileName = inFileName_arg.getValue();
        outFileName = outFileName_arg.getValue();
        print_ids = print_ids_arg.getValue();

        // reading the vars list
        auto vars_ = vars_arg.getValue();
        nev = (int) nev_arg.getValue();
        for (int iv = 0; iv < vars_.size(); iv++) {
            auto v = vars_[iv];
            if (v[0] == '[' && v[v.length() - 1] == ']') {
                v = std::regex_replace(v, std::regex(" "), "");
                v = std::regex_replace(v, std::regex("\\["), "");
                v = std::regex_replace(v, std::regex("\\]"), "");
                cout << "LIST OF ARGS" << endl;
                string delim = ",";
                size_t prev = 0, pos = 0;
                do {
                    pos = v.find(delim, prev);
                    if (pos == string::npos) pos = v.length();
                    auto token = v.substr(prev, pos - prev);
                    if (!token.empty()) vars.push_back(token);
                    prev = pos + delim.length();
                } while (pos < v.length() && prev < v.length());
            } else {
                vars.push_back(v);
            };
        };
    } catch (ArgException &e) {
        cerr << "error: " << e.error() << " at arg=" << e.argId() << endl;
    };

    cout << " Running with: " << endl;
    cout << "\t inFileName=" << inFileName << endl;
    cout << "\t outFileName=" << outFileName << endl;
    cout << " vars = [";
    for (int i = 0; i < vars.size(); ++i) {
        cout << vars[i] << " ";
    };
    cout << "]" << endl;
    cout << " nev = " << nev << endl;
    cout << " print_ids = " << print_ids << endl;
}

void init_input_fields(TTree *ntp) {
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

int char_to_ind(char c) {
    int ind = c - '0';
    if (ind < 0 || ind >= nTrk) {
        cout << "wrong particle number " << ind << endl;
        ::abort();
    };
    return ind;
}

TLorentzVector get_mom_from_arg(string var, int pos) {
    TLorentzVector P, _p;
    for (int i = pos; i < var.length(); ++i) {
        float fact = 1;
        if(var[i]=='m' && i<var.length()) {
            fact = -1;
            ++i;
        }
        int ind = char_to_ind(var[i]);
        _p.SetXYZT(fPx[ind], fPy[ind], fPz[ind], fE[ind]);
        P += fact*_p;
    };
    return P;
}

float calc_var(string var) {
    TLorentzVector P;
    if(var.substr(0,4)=="cth_") {
        P = get_mom_from_arg(var,4);
        return P.Z()/sqrt(P.X()*P.X()+P.Y()*P.Y()+P.Z()*P.Z());
    } else  if(var.substr(0,3)=="pT_" || var.substr(0,3)=="pt_") {
        P = get_mom_from_arg(var,3);
        return P.Pt();
    } else if (var.substr(0, 3) == "m2_") {
        P = get_mom_from_arg(var, 3);
        return P.M2();
    } else if (var.substr(0, 2) == "m_") {
        TLorentzVector P, _p;
        P = get_mom_from_arg(var, 2);
        return P.M();
    } else if (var.substr(0, 3) == "id_") {
        int ind = char_to_ind(var[3]);
        return pdgID[ind];
    } else if (var == "prob") {
        return fProb;
    } else {
        cout << "Unknown variable " << var << endl;
        ::abort();
    }
}

void read_event(TNtuple *tup, int iEv) {
    vector<float> values;
    for (int i = 0; i < vars.size(); ++i) {
        float x = calc_var(vars[i]);
        values.push_back(x);
    };
    tup->Fill(values.data());
}

int main(int argc, char **argv) {
    read_args(argc, argv);

    EvtPDL pdl;
    pdl.read("evt.pdl");

    TFile *in_file = new TFile(inFileName.c_str(), "READ");
    TFile *out_file = new TFile(outFileName.c_str(), "RECREATE");

    TTree *ntp = (TTree*) in_file->Get("ntp");
    init_input_fields(ntp);
    int nEv;
    if (nev < 0 || nev > ntp->GetEntries()) nEv = ntp->GetEntries();
    else nEv = nev;

    string fields = "";
    for (int i = 0; i < vars.size(); ++i) {
        fields += vars[i] + ":";
    };
    fields.pop_back();
    TNtuple *tup = new TNtuple("tup", "tup", fields.c_str());

    for (int iEv = 0; iEv < nEv; ++iEv) {
        ntp->GetEvent(iEv);
        if (iEv % (nEv / 10) == 0) cout << " iEv=" << iEv << endl;
        read_event(tup, iEv);
        if (print_ids) {
            for (int id = 0; id < nTrk; ++id) {
                cout << id << ":" << EvtPDL::name(EvtPDL::evtIdFromLundKC(pdgID[id])) << " ";
            };
            cout << endl;
        }
    };

    tup->Write();
    out_file->Close();
    in_file->Close();

    return 0;
}
