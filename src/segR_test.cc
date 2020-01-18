/* 
 * File:   segR_test.cc
 * Author: luchinsky
 *
 * Created on January 18, 2020, 11:12 AM
 */
#include "segR.h"
#include "EvtGenModels/EvtProbeModel.hh"
#include "EvtGenBase/EvtModel.hh"

using std::endl;
using std::ofstream;
using std::cout;




int main(int argc, char* argv[]) {
    TStopwatch timer;
    timer.Start();

    EvtModel &modellist = EvtModel::instance();
    modellist.registerModel(new EvtProbeModel());
    
    segR(argc, argv);
    
    //out.close();
    timer.Stop();
    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();
    printf("RealTime=%f seconds, CpuTime=%f seconds\n", rtime, ctime);

    return 1;
}
