################################################################################
 #    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    #
 #                                                                              #
 #              This software is distributed under the terms of the             # 
 #         GNU Lesser General Public Licence version 3 (LGPL) version 3,        #  
 #                  copied verbatim in the file "LICENSE"                       #
 ################################################################################
# - Try to find EvtGen instalation
# Once done this will define
#

MESSAGE(STATUS "Looking for HepMC ...")

FIND_PATH(HEPMC_INCLUDE_DIR NAMES HepMC/HepMCDefs.h PATHS
  /Users/luchinsky/Work/EvtGen/external/HepMC/include/			    
 /afs/cern.ch/user/a/aluchins/EvtGen/EvtGen/external/HepMC/include/
  /usr/local/include/
  /Users/alekseiluchinsky/miniconda3/include
  NO_DEFAULT_PATH
)


FIND_PATH(HEPMC_LIBRARY_DIR NAMES libHepMC.a PATHS
  ${SIMPATH}/lib
  /Users/luchinsky/Work/EvtGen/external/HepMC/lib/
  /usr/local/lib
 /afs/cern.ch/user/a/aluchins/EvtGen/EvtGen/external/HepMC/lib/
 /Users/alekseiluchinsky/miniconda3/lib
  NO_DEFAULT_PATH
)





if (HEPMC_INCLUDE_DIR AND HEPMC_LIBRARY_DIR)
   set(HEPMC_FOUND TRUE)
endif (HEPMC_INCLUDE_DIR AND HEPMC_LIBRARY_DIR)

if (HEPMC_FOUND)
  if (NOT HEPMC_FOUND_QUIETLY)
    MESSAGE(STATUS "Looking for HEPMC... - found ${HEPMC_LIBRARY_DIR}")
    SET(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} ${HEPMC_LIBRARY_DIR})
  endif (NOT HEPMC_FOUND_QUIETLY)
else (HEPMC_FOUND)
  if (HEPMC_FOUND_REQUIRED)
    message(FATAL_ERROR "Looking for HEPMC... - Not found")
  endif (HEPMC_FOUND_REQUIRED)
endif (HEPMC_FOUND)

