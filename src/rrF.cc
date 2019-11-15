/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "tclap/CmdLine.h"
#include <iostream>
#include <fstream>
#include "TFile.h"
#include "TNtuple.h"
#include "TH1F.h"
#include "TLorentzVector.h"
#include "EvtGenBase/EvtPDL.hh"
#include "EvtGenBase/EvtId.hh"
#include "EvtGenBase/EvtVector4R.hh"
#include "cut.h"
#include "RrfEvent.h"


using namespace TCLAP;
using namespace std;

// histogram parameters
vector<string> vars;
vector<float> min_list, max_list;
vector<int> nbins_list;
string evt_pdl_path;

// cuts
vector<cut*> cuts;

// command line parameters
string inFileName, outFileName;
int nev;
bool print_ids;

// input file fields
int nBins;
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

vector<string> split_string(string str, string sep) {
    char* cstr = const_cast<char*> (str.c_str());
    char* current;
    vector<std::string> arr;
    current = strtok(cstr, sep.c_str());
    while (current != NULL) {
        arr.push_back(current);
        current = strtok(NULL, sep.c_str());
    }
    return arr;
}

void add_var(string var) {
    string v = var;
    ReplaceStringInPlace(v, "(", ":");
    ReplaceStringInPlace(v, ")", ":");
    vector<string> vv = split_string(v, ":");
    // read var name
    if (vv.size() < 1) {
        cout << "WR0NG variable " << var << "!" << endl;
    } else vars.push_back(vv[0]);
    // read nbins
    if (vv.size() < 2) {
        nbins_list.push_back(-1);
    } else {
        nbins_list.push_back(atoi(vv[1].c_str()));
    };
    // read min
    if (vv.size() < 3) {
        min_list.push_back(1);
    } else {
        min_list.push_back(atof(vv[2].c_str()));
    };
    // read max
    if (vv.size() < 4) {
        max_list.push_back(0);
    } else {
        max_list.push_back(atof(vv[3].c_str()));
    };
}

void read_hst_args(vector<string> vars_) {
    for (int iv = 0; iv < vars_.size(); iv++) {
        auto v = vars_[iv];
        if (v[0] == '[' && v[v.length() - 1] == ']') {
            ReplaceStringInPlace(v, " ", "");
            ReplaceStringInPlace(v, "[", "");
            ReplaceStringInPlace(v, "]", "");
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
        TCLAP::CmdLine cmd("Reads ROOT file\n (c) Alexey Luchinsky", ' ', "0.1");
        ValueArg<string> inFileName_arg("i", "in", "input ROOT file", false, "evtOutput.root", "string", cmd);
        ValueArg<string> outFileName_arg("o", "out", "output ROOT file", false, "out.root", "string", cmd);
        MultiArg<string> vars_arg("v", "var", "variable to be saved, e.g. m2_12. "
                "You can also specify number of bins, min and max values like m2_12(10, 1.2, 2.9)", true, "string", cmd);
        ValueArg<string> evt_pdl_path_arg("e", "evt_pdl", "evt.pdl file", false, "evt.pdl", "string", cmd);
        ValueArg<float> nev_arg("n", "nev", "Number of events to be read (negative if all events should be read)", false, -1, "float", cmd);
        SwitchArg print_ids_arg("p", "print-ids", "should we print ids of the particles", false);
        cmd.add(print_ids_arg);
        SwitchArg save_hst_arg("s", "save", "Should we save histograms as text files?", false);
        cmd.add(save_hst_arg);
        ValueArg<int> nBins_arg("b", "bins", "Number of bins in the histogrm", false, 50, "int", cmd);
        MultiArg<string> cuts_arg("c", "cut", "cuts", false, "", cmd);

        cmd.parse(argc, argv);
        inFileName = inFileName_arg.getValue();
        outFileName = outFileName_arg.getValue();
        print_ids = print_ids_arg.getValue();
        save_hst = save_hst_arg.getValue();
        nBins = nBins_arg.getValue();
        nev = (int) nev_arg.getValue();
        evt_pdl_path = evt_pdl_path_arg.getValue();

        // reading the vars list
        read_hst_args(vars_arg.getValue());

        // reading cuts
        vector<string> cuts_string = cuts_arg.getValue();
        for (string s : cuts_string) {
            ReplaceStringInPlace(s, "&", ",");
            if (s.find(',') != string::npos) {
                for (string v : split_string(s, ",")) {
                    cuts.push_back(new cut(v));
                }
            } else {
                cuts.push_back(new cut(s));
            };
        }
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
    cout << "\t nBins_list=[";
    for (int i = 0; i < nbins_list.size(); ++i) {
        cout << nbins_list[i] << " ";
    };
    cout << "]" << endl;
    cout << "\t min_list=[";
    for (int i = 0; i < min_list.size(); ++i) {
        cout << min_list[i] << " ";
    };
    cout << "]" << endl;
    cout << "\t max_list=[";
    for (int i = 0; i < max_list.size(); ++i) {
        cout << max_list[i] << " ";
    };
    cout << "]" << endl;
    cout << "\t nev = " << nev << endl;
    cout << "\t print_ids = " << print_ids << endl;
    cout << "\t save_hst = " << save_hst << endl;
    cout << "\t nBins = " << nBins << endl;
    cout << "\t evt_pdl_path = " << evt_pdl_path << endl;
    cout << "\t number of cuts: " << cuts.size() << endl;
    for (cut *c : cuts) {
        cout << "|"<<c->get_var()<< "|"<< endl;
    }
}



double cos_between(EvtVector4R p1, EvtVector4R p2) {
    double mag1 = 0, mag2 = 0, p1p2 = 0;
    for (int i = 1; i <= 3; ++i) {
        mag1 += p1.get(i) * p1.get(i);
        mag2 += p2.get(i) * p2.get(i);
        p1p2 += p1.get(i) * p2.get(i);
    }
    return p1p2 / sqrt(mag1) / sqrt(mag2);
}

void print_vec(std::string title, EvtVector4R k) {
    cout << title << "=" << k << "; m = " << k.mass() << ";\n";
}

float calc_var(RrfEvent event, string var) {
    EvtVector4R P;
    if (var.substr(0, 4) == "cth_") {
        P = event.get_mom_from_arg(var, 4, var.length());
        return P.get(3) / sqrt(P.get(1) * P.get(1) + P.get(2) * P.get(2) + P.get(3) * P.get(3));
    } else if (var.substr(0, 5) == "cos0_") {
        //        cout << "calc_var: var = " << var << endl;
        size_t pos2 = var.find("_", 6);
        EvtVector4R k1 = event.get_mom_from_arg(var, 5, pos2);
        //        print_vec("mom1", k1);
        EvtVector4R k2 = event.get_mom_from_arg(var, pos2 + 1, var.length());
        //        print_vec("mom2", k2);
        EvtVector4R k10 = k1;
        k10.applyBoostTo(k2, true);
        return cos_between(k2, k10);
    } else if (var.substr(0, 4) == "cos_" && var.length() == 7) {
        EvtVector4R p1 = event.get_mom_from_arg(var, 4, 5);
        EvtVector4R p2 = event.get_mom_from_arg(var, 6, 7);
        return cos_between(p1, p2);
    } else if (var.substr(0, 3) == "pT_" || var.substr(0, 3) == "pt_") {
        P = event.get_mom_from_arg(var, 3, var.length());
        return sqrt(P.get(1) * P.get(1) + P.get(2) * P.get(2));
    } else if (var.substr(0, 3) == "px_" || var.substr(0, 3) == "pX_") {
        P = event.get_mom_from_arg(var, 3, var.length());
        return P.get(1);
    } else if (var.substr(0, 3) == "py_" || var.substr(0, 3) == "pY_") {
        P = event.get_mom_from_arg(var, 3, var.length());
        return P.get(2);
    } else if (var.substr(0, 3) == "pz_" || var.substr(0, 3) == "pZ_") {
        P = event.get_mom_from_arg(var, 3, var.length());
        return P.get(3);
    } else if (var.substr(0, 2) == "E_" || var.substr(0, 2) == "e_") {
        P = event.get_mom_from_arg(var, 2, var.length());
        return P.get(0);
    } else if (var.substr(0, 3) == "m2_") {
        P = event.get_mom_from_arg(var, 3, var.length());
        return P.mass2();
    } else if (var.substr(0, 2) == "m_") {
        P = event.get_mom_from_arg(var, 2, var.length());
        return P.mass();
    } else if (var.substr(0, 3) == "id_") {
        int ind = event.char_to_ind(var[3]);
        return event.pdgID[ind];
    } else if (var == "prob") {
        return event.fProb;
    } else {
        cout << "Unknown variable \" \"" << var << endl;
        ::abort();
    }
}

bool read_event(RrfEvent event, TNtuple *tup, int iEv) {
    for (cut* c : cuts) {
        if (!c->is_ok(event)) {
            return false;
        }
    };
    vector<float> values;
    for (int i = 0; i < vars.size(); ++i) {
        float x = calc_var(event, vars[i]);
        values.push_back(x);
    };
    tup->Fill(values.data());
    return true;
}

int main(int argc, char **argv) {
    cout << "rrf.exe, (c) Alexey Luchinsky" << endl;
    read_args(argc, argv);

    EvtPDL pdl;
    pdl.read(evt_pdl_path.c_str());

    TFile *in_file = new TFile(inFileName.c_str(), "READ");
    RrfEvent event;
    TFile *out_file = new TFile(outFileName.c_str(), "RECREATE");

    TTree *ntp = (TTree*) in_file->Get("ntp");
    event.init_input_fields(ntp);
    int nEv;
    if (nev < 0 || nev > ntp->GetEntries()) nEv = ntp->GetEntries();
    else nEv = nev;

    string fields = "";
    for (int i = 0; i < vars.size(); ++i) {
        fields += vars[i] + ":";
    };
    fields.pop_back();
    TNtuple *tup = new TNtuple("tup", "tup", fields.c_str());

    int passed = 0;
    for (int iEv = 0; iEv < nEv; ++iEv) {
        ntp->GetEvent(iEv);
        if (iEv % (nEv / 10) == 0) cout << " iEv=" << iEv << endl;
        if (read_event(event, tup, iEv)) {
            passed++;
        };
        if (print_ids) {
            for (int id = 0; id < event.nTrk; ++id) {
                cout << id << ":" << EvtPDL::name(EvtPDL::evtIdFromLundKC(event.pdgID[id])) << " ";
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

    // delete cuts
    for (auto & c : cuts) {
        delete c;
    }
    cout << passed << " events (" << 100. * passed / nEv << ") passed the cuts " << endl;
    return 0;
}
