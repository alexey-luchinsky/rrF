/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "tclap/CmdLine.h"
#include <iostream>
#include <regex>
#include <fstream>
#include "TFile.h"
#include "TNtuple.h"
#include "TH1F.h"
#include "TLorentzVector.h"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtId.hh"

using namespace TCLAP;
using namespace std;

// histogram parameters
vector<string> vars;
vector<float> min_list, max_list;
vector<int> nbins_list;


// command line parameters
string inFileName, outFileName;
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
int nBins;
Double_t fProb;
bool save_hst;

void saveHST(TNtuple *tup, string var, string fileName, double min_ = 1, double max_ = -1, int bins_ = -1) {
    cout << " Saving " << var << " to file " << fileName << endl;
    const char *var_name = var.c_str();
    double min = min_, max = max_;
    if (min > max) {
        min = tup->GetMinimum(var.c_str()), max = tup->GetMaximum(var.c_str());
    };
    int bins = bins_;
    if (bins < 0) bins = nBins;
    TH1F *histogram = new TH1F(var_name, var_name, bins, min, max);
    histogram->Sumw2();
    tup->Project(var_name, var.c_str());
    ofstream file;
    file.open(fileName);
    for (int i = 1; i <= histogram->GetNbinsX(); i++)
        file << setiosflags(ios::scientific) << histogram->GetBinCenter(i) <<
        " " << setiosflags(ios::scientific) << histogram->GetBinContent(i) / histogram->GetBinWidth(i) <<
        " " << setiosflags(ios::scientific) << histogram->GetBinError(i) / histogram->GetBinWidth(i) << endl;

    //histogram->Delete();
    histogram->Write();
    file.close();
}

vector<string> split_string(string str, string sep){
    char* cstr=const_cast<char*>(str.c_str());
    char* current;
    vector<std::string> arr;
    current=strtok(cstr,sep.c_str());
    while(current != NULL){
        arr.push_back(current);
        current=strtok(NULL, sep.c_str());
    }
    return arr;
}

void add_var(string var) {
    string v = var;
    v = regex_replace(v, regex("\\("),":");
    v = regex_replace(v, regex("\\)"),":");
    vector<string> vv = split_string(v, ":");
    cout<<"vv]="<<vv.size()<<endl;
    // read var name
    if(vv.size()<1) {
        cout<<"WR0NG variable "<<var<<"!"<<endl;
    } else vars.push_back(vv[0]);
    // read nbins
    if(vv.size()<2) {
        nbins_list.push_back(-1);
    } else {
        nbins_list.push_back(atoi(vv[1].c_str()));
    };
    // read min
    if(vv.size()<3) {
        min_list.push_back(1);
    } else {
        min_list.push_back(atof(vv[2].c_str()));
    };
    // read max
    if(vv.size()<4) {
        max_list.push_back(0);
    } else {
        max_list.push_back(atof(vv[3].c_str()));
    };
}

void read_hst_args(vector<string> vars_) {
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
                if (!token.empty()) add_var(token);
                prev = pos + delim.length();
            } while (pos < v.length() && prev < v.length());
        } else {
            add_var(v);
        };
    };
}


void read_args(int argc, char **argv) {
    try {
        TCLAP::CmdLine cmd("Reads ROOT file", ' ', "0.1");
        ValueArg<string> inFileName_arg("i", "in", "input ROOT file", false, "evtOutput.root", "string", cmd);
        ValueArg<string> outFileName_arg("o", "out", "output ROOT file", false, "out.root", "string", cmd);
        MultiArg<string> vars_arg("v", "var", "variable to be saved, e.g. m2_12. "
        "You can also specify number of bins, min and max values like m2_12(10, 1.2, 2.9)", true, "string", cmd);
        ValueArg<float> nev_arg("n", "nev", "Number of events to be read (negative if all events should be read)", false, -1, "float", cmd);
        SwitchArg print_ids_arg("p", "print-ids", "should we print ids of the particles", false);
        cmd.add(print_ids_arg);
        SwitchArg save_hst_arg("s", "save", "Should we save histograms as text files?", false);
        cmd.add(save_hst_arg);
        ValueArg<int> nBins_arg("b", "bins", "Number of bins in the histogrm", false, 50, "int", cmd);

        cmd.parse(argc, argv);
        inFileName = inFileName_arg.getValue();
        outFileName = outFileName_arg.getValue();
        print_ids = print_ids_arg.getValue();
        save_hst = save_hst_arg.getValue();
        nBins = nBins_arg.getValue();
        nev = (int) nev_arg.getValue();

        // reading the vars list
        read_hst_args(vars_arg.getValue());
    } catch (ArgException &e) {
        cerr << "error: " << e.error() << " at arg=" << e.argId() << endl;
    };

    cout << " Running with: " << endl;
    cout << "\t inFileName=" << inFileName << endl;
    cout << "\t outFileName=" << outFileName << endl;
    cout << "\t vars = [";
    for (int i = 0; i < vars.size(); ++i) {
        cout << vars[i] << " ";
    };
    cout << "]" << endl;
    cout<<"\t nBins_list=[";
    for (int i = 0; i < nbins_list.size(); ++i) {
        cout << nbins_list[i] << " ";
    };
    cout << "]" << endl;
    cout<<"\t min_list=[";
    for (int i = 0; i < min_list.size(); ++i) {
        cout << min_list[i] << " ";
    };
    cout << "]" << endl;
    cout<<"\t max_list=[";
    for (int i = 0; i < max_list.size(); ++i) {
        cout << max_list[i] << " ";
    };
    cout << "]" << endl;
    cout << "\t nev = " << nev << endl;
    cout << "\t print_ids = " << print_ids << endl;
    cout << "\t save_hst = " << save_hst << endl;
    cout << "\t nBins = " << nBins << endl;
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
        if (var[i] == 'm' && i < var.length()) {
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
    if (var.substr(0, 4) == "cth_") {
        P = get_mom_from_arg(var, 4);
        return P.Z() / sqrt(P.X() * P.X() + P.Y() * P.Y() + P.Z() * P.Z());
    } else if (var.substr(0, 3) == "pT_" || var.substr(0, 3) == "pt_") {
        P = get_mom_from_arg(var, 3);
        return P.Pt();
    } else if (var.substr(0, 2) == "E_" || var.substr(0, 2) == "e_") {
        P = get_mom_from_arg(var, 2);
        return P.E();
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

    if (save_hst) {
        for (int i = 0; i < vars.size(); ++i) {
            saveHST(tup, vars[i], vars[i] + ".txt", min_list[i], max_list[i], nbins_list[i]);
        };
    };

    tup->Write();
    out_file->Close();
    in_file->Close();

    return 0;
}
