**rrF** (from ReadRootFile) tool helps the user to read ROOT files created by EvtGen program _simpleEvtGenRO_

# Short Description

Suppose we have generated some tau -> e anti-nu-e nu-tau decays running the command

   **./simpleEvtGenRO.exe tau- tau_enu.dec 100000**

Distribution over electron's energy can be extracted by the command

   **./rrF.exe -i evtOutput.root -o out.root -v e_1**

# Long Description

This tool is rather flexible and can be run with varios command line arguments. The list of these arguments can be obtained with the command

   **./rrF.exe -h**

Almost all arguments are self-explainable, so we will say some words about variables specification only.

Variables to be saved can be specified using argument **-v**. It's general format is var_part(nBins:min:max), where

* var is the name of the variable. Currently only the following variants are implemented:
  * "m" --- mass of the particles system 
  * "m2" --- squared mass
  * "pT" --- transvers momentum
  * "e" --- energy
  * "cth" -- Cotangence
  * id --- PDG identification number
* part is the sting specifying system of particles. Every number in this string corresponds to the particle with this number in the decay file (zero stands for the decaying particle). If the letter m is preceeding the number, the corresponding momentum is taken with minus sign. For example, in the case of "tau- to e- anti-nu-e nu_tau" decay variable "m2_12m3" is (pE+pnuE-pnuTau)^2.
* nBins, min, max are requested number of bins, minimum and maximum values of the histogram

Several histograms can be saved either by giving "-v" arguments more than once, or in the list form like **-v "[m2_12, pT_3]"

# Files and Installation

Included files are:

* rrF.cc --- source of the tool
* simpleEvtGenRo.cc --- file to produce ROOT,  taken from EvtGen distribution
* Makefile
* evt.pdl (required by simpleEvtGenRo) --- list of the particles,  taken from EvtGen distribution
* tau_enu.dec --- sample decay file

Installation is pretty straightforward. You need to have ROOT, EvtGen, and HEPMC installed on your computer. After correcting the paths to the corresponding libraries in the Makefile simply use **make**.

