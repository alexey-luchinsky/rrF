/* 
 * File:   segR_test.cc
 * Author: luchinsky
 *
 * Created on January 18, 2020, 11:12 AM
 */
#include "segR.h"


using std::endl;
using std::ofstream;
using std::cout;




int main(int argc, char* argv[]) {
    TStopwatch timer;
    timer.Start();

    segR gen(argc, argv);
    gen.run();
    
    //out.close();
    timer.Stop();
    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();
    printf("RealTime=%f seconds, CpuTime=%f seconds\n", rtime, ctime);

    return 1;
}
