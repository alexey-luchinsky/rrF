#include <iostream>
#include "TFile.h"
#include "TNtuple.h"
#include "TLorentzVector.h"
#include "EvtGenBase/EvtPDL.hh"
#include <string>  // std:to_string
using namespace std;

#define MAX 100

Int_t ev = 0;
Int_t nLine[MAX], pdgID[MAX], nDau[MAX], nM1[MAX], nM2[MAX], nDF[MAX], nDL[MAX];
Double_t fPx[MAX], fPy[MAX], fPz[MAX], fE[MAX];
Double_t fVx[MAX], fVy[MAX], fVz[MAX], fT[MAX];
Double_t fTht[MAX], fM[MAX], fP[MAX], fPt[MAX];
Int_t nTrk = 0;


int main(int argc, char **argv) {

    EvtPDL pdl;
    pdl.read("evt.pdl");
    

    cout << "./readRootFile.exe [inFile=evtOutput.root] [i1=1] [i2=2] [out=out_<i1><i2>_<inFile>]" << endl;
    string inFileName = "evtOutput.root";
    int i1 = 1, i2 = 2;
    if (argc > 1) inFileName = argv[1];
    if (argc > 2) i1 = atoi(argv[2]);
    if (argc > 3) i2 = atoi(argv[3]);
    string outFileName = "out_"+to_string(i1)+to_string(i2)+"_"+inFileName;
    if (argc > 4) outFileName = argv[4];
    cout << "Running with parameters:" << endl;
    cout << " inFileName = " << inFileName << endl;
    cout << " i1 = " << i1 << endl;
    cout << " i2 = " << i2 << endl;
    cout << " outFileName = " << outFileName << endl;


    TFile *inFile = new TFile(inFileName.c_str(), "READ"),
            *outFile = new TFile(outFileName.c_str(), "RECREATE");

    TTree *ntp = (TTree*) inFile->Get("ntp");
    TNtuple *tup = new TNtuple("tup", "tup", "m2");
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



    ntp->GetEvent(0);
    EvtId id1 = EvtPDL::evtIdFromLundKC(pdgID[i1]);
    EvtId id2 = EvtPDL::evtIdFromLundKC(pdgID[i2]);
    cout << " id1=" << EvtPDL::name(id1) << " id2=" << EvtPDL::name(id2) << endl;

    for (int iEv = 0; iEv < ntp->GetEntries(); ++iEv) {
        ntp->GetEvent(iEv);
        TLorentzVector p1(fPx[i1], fPy[i1], fPz[i1], fE[i1]);
        TLorentzVector p2(fPx[i2], fPy[i2], fPz[i2], fE[i2]);
        double m2 = (p1 + p2).Mag2();
        tup->Fill(m2);
    }

    inFile->Close();
    tup->Write();
    outFile->Save();
    outFile->Close();
    return 0;
}
