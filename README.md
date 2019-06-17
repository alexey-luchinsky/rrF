**rrF** (from ReadRootFile) tool helps the user to read ROOT files created by EvtGen program _simpleEvtGenRO_

# Short Description

Suppose we have generated some tau -> e anti-nu-e nu-tau decays running the command
**./simpleEvtGenRO.exe tau- tau_enu.dec 100000**

Distribution over electron's energy can be extracted by the command
**./rrF.exe -i evtOutput.root -o out.root -v e_1**

# Long Description

This tool is rather flexible and can be run with varios command line arguments. The list of these arguments can be obtained with the command
**./rrF.exe -h**
