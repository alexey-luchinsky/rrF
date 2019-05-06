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

using namespace TCLAP;
using namespace std;

// command line params
string inFileName, outFileName, vars;

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
        TCLAP::CmdLine cmd("Reads ROOT file",' ',"0.1");
        ValueArg<string> inFileName_arg("i","in","input ROOT file",false, "evtOutput.root", "string", cmd);
        ValueArg<string> outFileName_arg("o","out","output ROOT file",false, "out.root", "string", cmd);
        ValueArg<string> vars_arg("v","var","variable to be saved",false, "m12", "string", cmd);
        
        cmd.parse(argc, argv);
        inFileName = inFileName_arg.getValue();
        outFileName = outFileName_arg.getValue();
        vars = vars_arg.getValue();
    }
    catch(ArgException &e) {
        cerr<<"error: "<<e.error()<<" at arg="<<e.argId()<<endl;
    };
    
    cout<<" Running with: "<<endl;
    cout<<"\t inFileName="<<inFileName<<endl;
    cout<<"\t outFileName="<<outFileName<<endl;
    cout<<"\t vars="<<vars<<endl;    
}

void init_input_fields(TNtuple *ntp) {
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

int main(int argc, char **argv) {
    read_args(argc, argv);
    
    TFile *in_file = new TFile(inFileName.c_str(),"READ");
    TFile *out_file = new TFile(outFileName.c_str(),"RECREATE");
    TNtuple *tup = new TNtuple("tup","tup","m2");
    tup->Fill(1);
    
    tup->Write();
//    out_file->Save();
    out_file->Close();
    in_file->Close();
    
    return 0;
}
