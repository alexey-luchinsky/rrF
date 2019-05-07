/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "tclap/CmdLine.h"
#include <iostream>
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

// input file fields
#define MAX 100
Int_t ev = 0;
Int_t nLine[MAX], pdgID[MAX], nDau[MAX], nM1[MAX], nM2[MAX], nDF[MAX], nDL[MAX];
Double_t fPx[MAX], fPy[MAX], fPz[MAX], fE[MAX];
Double_t fVx[MAX], fVy[MAX], fVz[MAX], fT[MAX];
Double_t fTht[MAX], fM[MAX], fP[MAX], fPt[MAX];
Int_t nTrk = 0;

void read_args(int argc, char **argv) {
    try {
        TCLAP::CmdLine cmd("Reads ROOT file", ' ', "0.1");
        ValueArg<string> inFileName_arg("i", "in", "input ROOT file", false, "evtOutput.root", "string", cmd);
        ValueArg<string> outFileName_arg("o", "out", "output ROOT file", false, "out.root", "string", cmd);
        MultiArg<string> vars_arg("v", "var", "variable to be saved, e.g. m2_12", true, "string", cmd);

        cmd.parse(argc, argv);
        inFileName = inFileName_arg.getValue();
        outFileName = outFileName_arg.getValue();
        vars = vars_arg.getValue();
    } catch (ArgException &e) {
        cerr << "error: " << e.error() << " at arg=" << e.argId() << endl;
    };

    cout << " Running with: " << endl;
    cout << "\t inFileName=" << inFileName << endl;
    cout << "\t outFileName=" << outFileName << endl;
    //    cout << "\t vars=" << vars << endl;
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
}

float calc_var(string var) {
    if (var.substr(0, 3) == "m2_") {
        TLorentzVector P, _p;
        for (int i = 3; i < var.length(); ++i) {
            int ind = var[i] - '0';
            _p.SetXYZT(fPx[ind], fPy[ind], fPz[ind], fE[ind]);
            P += _p;
        }
        return P.M2();
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
    tup->Fill( values.data() );
}

int main(int argc, char **argv) {
    read_args(argc, argv);

    TFile *in_file = new TFile(inFileName.c_str(), "READ");
    TFile *out_file = new TFile(outFileName.c_str(), "RECREATE");

    TTree *ntp = (TTree*) in_file->Get("ntp");
    int nEv = ntp->GetEntries();
    init_input_fields(ntp);

    string fields = "";
    for (int i = 0; i < vars.size(); ++i) {
        fields += vars[i] + ":";
    };
    fields.pop_back();
    cout<<" fields = \""<<fields<<"\""<<endl;
    TNtuple *tup = new TNtuple("tup", "tup", fields.c_str());

    for (int iEv = 0; iEv < nEv; ++iEv) {
        ntp->GetEvent(iEv);
        if (iEv % (nEv / 10) == 0) cout << " iEv=" << iEv << endl;
        read_event(tup, iEv);
    };

    tup->Write();
    out_file->Close();
    in_file->Close();

    return 0;
}
