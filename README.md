**rrF** (from ReadRootFile) tool helps the user to read ROOT files created by EvtGen program _simpleEvtGenRO_

(c) Alexey Luchinsky

# Short Description

Suppose we have generated some tau -> e anti-nu-e nu-tau decays running the command from build/ directory

    ./simpleEvtGenRO.exe tau- ../src/tau_enu.dec 100000

Distribution over electron's energy can be extracted by the command

    ./rrF.exe -i evtOutput.root -o out.root -v e_1

# Long Description

This tool is rather flexible and can be run with varios command line arguments. The list of these arguments can be obtained with the command

    ./rrF.exe -h

Almost all arguments are self-explainable, so we will say some words about variables specification only.

Variables to be saved can be specified using argument **-v**. Possible varianles can be divided into two
classes:

  * event variables
  * particle variables

The variables of the first class describe the event as a whole. The following options are available:

* prob: the probability of the event
* nt: total number of tracks in the event. Note that this number has little to do with the multiplicity
since the virstual particles are also counted. It can be used, however, to discriminated differemt channels
from each other.

 
The  general format of the particles-specific variables is var_part(nBins:min:max), where

* var is the name of the variable. Currently only the following variants are implemented:
  * "m" --- mass of the particles system 
  * "m2" --- squared mass
  * "pT" --- transvers momentum
  * "px" --- x-component of the momentum 
  * "py" --- y-component of the momentum 
  * "pz" --- z-component of the momentum 
  * "e" --- energy 
  * "cth" -- Cotangence
  * id --- PDG identification number
  * alternatively you can use var 'cos\_i\_j', where i and j are the
    number of particles. The result will be the cosine of the momenta
    of the corresponding particles in lab frame.
  * the other variant is cos0\_i\_j, wich gives the cosine of the
      polar angle of particles' set j in the restframe of set i with
      respect with i's direction in root particle rest frame
* part is the srting specifying system of particles. Every number in this string corresponds to the particle with this number in the decay file (zero stands for the decaying particle, see also the descriptor below). If the letter m is preceeding the number, the corresponding momentum is taken with minus sign. For example, in the case of "tau- to e- anti-nu-e nu_tau" decay variable "m2_12m3" is (pE+pnuE-pnuTau)^2.
* nBins, min, max are requested number of bins, minimum and maximum values of the histogram

Several histograms can be saved either by giving "-v" arguments more than once, or in the list form like **-v "[m2_12, pT_3]"

You can also impose some cuts on the listed above variables. This can be done by specifying -c (or --cuts) argument. For example,

    ./rrF.exe -v E_1 -c "E_2<0.3"

means that the energy of the 2nd particle should be smaller than 0.3 GeV. The allowed operations are ">", ">", and "=". 
In the latter case the RHS can be specified with an error separated by "+-" symbols (with the default value being 1e-5).
All listed above variables can be used, but keep in mind that the variable should be on the left hand side of the comparison (i.e. "E_1<0.3" is OK, but "0.3>E_1" is wrong). You can combine (logical AND) several cuts by supplying several -c switches or by joining then with "&" operator. As a result,

    ./rrF.exe -v E_1 -c "E_2<0.3" -c "E_2>0.1"

and

    ./rrF.exe -v E_1 -c "E_2<0.3 & E_2>0.1"

will have the same effect. The same result can be achieved with the more simple notation

    ./rrF.exe -v E_1 -c "E_2=0.2+-0.1"


If there are too many particles in the final state or you do not want to think about the order if the final particles in the decay file, you can specify the ones you are interested in using the desciptor. For example, if you run the command

    ./rrF.exe -v E_0 -v m_01 -d "tau- -> nu_tau ^e- ^anti-nu_e"

where 0-th and 1-st momenta will correspond to momenta of e- and anti-nu_e respectively (the names of these oarticles are marked with ^ character, the momenta are numbered in acordance with the order of these enties). Identical particles currelntly are not processed correctly.

# Files and Installation

Included files are:

* src/rrF.cc --- source of the tool
* src/cut.cc, src/cut.h --- class that decribes the cut on the variable
* src/simpleEvtGenRo.cc --- file to produce ROOT,  taken from EvtGen distribution
* src/a.for --- empty file needed to link with EvtGen
* src/evt.pdl (required by simpleEvtGenRo) --- list of the particles,  taken from EvtGen distribution
* src/tau_enu.dec --- sample decay file
* Find*.cmake --- cmake scripts to find different packages



Installation is pretty straightforward. You need to have ROOT, EvtGen, and HEPMC and TCLAP (http://tclap.sourceforge.net/) installed on your computer. After correcting the paths in Find*.cmake files do the following:

**cd build/**

**cmake ../**

**make**


