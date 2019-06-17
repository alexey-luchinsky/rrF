_rrF_ (from **r**ead**r**oot**F**ile) tool helps the user to read ROOT files created by EvtGen program _simpleEvtGenRO_

# Short Description

Suppose we have generated some tau -> e anti-nu-e nu-tau decays running the command
**./simpleEvtGenRO.exe tau- tau_enu.dec 100000**

Distribution over electron's energy can be extracted by the command
**./rrF.exe -i evtOutput.root -o out.root -v e_1**

