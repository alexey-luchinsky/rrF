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
#include "EvtGenBase/EvtParticleFactory.hh"
#include "EvtGenBase/EvtParticle.hh"

#include "cut.h"
#include "RrfEvent.h"
#include "RrfVar.h"


using namespace TCLAP;
using namespace std;




// descriptor
string descriptor_string;
std::vector<int> desciptor_vec;

// histogram parameters
vector<RrfVar *> vars;
vector<float> min_list, max_list;
vector<int> nbins_list;
string evt_pdl_path;

// cuts
vector<cut*> cuts;

// command line parameters
string inFileName, outFileName;
int nev;
bool print_ids;
int max_daug_number;


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
    } else vars.push_back(varFactory(vv[0], &desciptor_vec));
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
                "You can also specify number of bins, min and max values like m2_12(10, 1.2, 2.9)", false, "string", cmd);
        ValueArg<string> evt_pdl_path_arg("e", "evt_pdl", "evt.pdl file", false, "evt.pdl", "string", cmd);
        ValueArg<float> nev_arg("n", "nev", "Number of events to be read (negative if all events should be read)", false, -1, "float", cmd);
        SwitchArg print_ids_arg("p", "print-ids", "should we print ids of the particles", false);
        cmd.add(print_ids_arg);
        SwitchArg save_hst_arg("s", "save", "Should we save histograms as text files?", false);
        cmd.add(save_hst_arg);
        ValueArg<int> nBins_arg("b", "bins", "Number of bins in the histogrm", false, 50, "int", cmd);
        MultiArg<string> cuts_arg("c", "cut", "cuts", false, "", cmd);
        ValueArg<string> descriptor_arg("d", "descriptor", "decay descriptor", false, "", "string", cmd);
        ValueArg<int> all_mass_arg("a", "all_mass", "add all mass pairs to variables, argument is the max daughter number", false, 0, "int", cmd);
        


        cmd.parse(argc, argv);
        inFileName = inFileName_arg.getValue();
        outFileName = outFileName_arg.getValue();
        print_ids = print_ids_arg.getValue();
        save_hst = save_hst_arg.getValue();
        nBins = nBins_arg.getValue();
        nev = (int) nev_arg.getValue();
        evt_pdl_path = evt_pdl_path_arg.getValue();

        // analyzing the descriptor       
        descriptor_string = descriptor_arg.getValue();

        // reading the vars list
        read_hst_args(vars_arg.getValue());
        max_daug_number = all_mass_arg.getValue();
        if(max_daug_number>1) 
        {
          for(int i1=1; i1<=max_daug_number; ++i1) 
          {
            for(int i2=i1+1; i2<=max_daug_number; ++i2) 
            {
              add_var("m_"+std::to_string(i1)+std::to_string(i2));
            }
          }
          
        };
        
        

        // reading cuts
        vector<string> cuts_string = cuts_arg.getValue();
        for (string s : cuts_string) {
            ReplaceStringInPlace(s, "&", ",");
            if (s.find(',') != string::npos) {
                for (string v : split_string(s, ",")) {
                    cuts.push_back(new cut(v, desciptor_vec));
                }
            } else {
                cuts.push_back(new cut(s, desciptor_vec));
            };
        }
    } catch (ArgException &e) {
        cerr << "error: " << e.error() << " at arg=" << e.argId() << endl;
    };

    cout << " Running with: " << endl;
    cout << "\t inFileName=" << inFileName << endl;
    cout << "\t outFileName=" << outFileName << endl;
    cout << "\t max_daug_number=" << max_daug_number << endl;
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
        cout << "|" << c->get_var() << "|" << endl;
    }
    cout << "\t descriptor = " << descriptor_string << endl;
}

void print_vec(std::string title, EvtVector4R k) {
    cout << title << "=" << k << "; m = " << k.mass() << ";\n";
}

bool read_event(RrfEvent *event, TNtuple *tup, int iEv) {
    for (cut* c : cuts) {
        if (!c->is_ok(event)) {
            return false;
        }
    };
    vector<float> values;
    for (int i = 0; i < vars.size(); ++i) {
        float x = vars[i]->getValue(event);
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
    RrfEvent *event = new RrfEvent;
    TFile *out_file = new TFile(outFileName.c_str(), "RECREATE");

    TTree *ntp = (TTree*) in_file->Get("ntp");
    event->init_input_fields(ntp);
    int nEv;
    if (nev < 0 || nev > ntp->GetEntries()) nEv = ntp->GetEntries();
    else nEv = nev;

    string fields = "";
    for (int i = 0; i < vars.size(); ++i) {
        fields += vars[i]->to_string() + ":";
    };
    fields.pop_back();
    TNtuple *tup = new TNtuple("tup", "tup", fields.c_str());

    vector<string> ss = split_string(descriptor_string, " ");
    EvtId evt_id;
    for (string d : ss) {
        if (d[0] == '^') {
            evt_id = EvtPDL::getId(d.substr(1));
            int id = EvtPDL::getStdHep(evt_id);
            //            int id = 0;
            desciptor_vec.push_back(id);
        }
    }
    cout << " desciptor_vec = {";
    for (int i : desciptor_vec) {
        cout << " " << i;
    }
    cout << "}" << endl;



    int passed = 0;
    for (int iEv = 0; iEv < nEv; ++iEv) {
        ntp->GetEvent(iEv);
        if (iEv % (nEv / 10) == 0) cout << " iEv=" << iEv << endl;
        if (read_event(event, tup, iEv)) {
            passed++;
        };
        if (print_ids) {
            for (int id = 0; id < event->nTrk; ++id) {
                cout << id << ":" << EvtPDL::name(EvtPDL::evtIdFromLundKC(event->pdgID[id])) << " ";
            };
            cout << endl;
        }
    };

    if (save_hst) {
        for (int i = 0; i < vars.size(); ++i) {
            saveHST(tup, vars[i]->to_string(), vars[i]->to_string() + ".txt", min_list[i], max_list[i], nbins_list[i]);
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
