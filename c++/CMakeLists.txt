##Test
MESSAGE("Hello, world")
IF(DEFINED APPLE)
    MESSAGE("APPLE")
ENDIF(DEFINED APPLE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_OSX_DEPLOYMENT_TARGET "")
INCLUDE(SetCompileFlag.cmake)

cmake_minimum_required(VERSION 2.6)
project(directPsi CXX Fortran)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

## include CERN ROOT
include(FindROOT.cmake)
MESSAGE(${ROOT_LIBRARIES})
include(FindEvtGen.cmake)
include(FindHepMC.cmake)


## set fortran flags
SET_COMPILE_FLAG(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS}"
                 Fortran "-fno-automatic")




set(CMAKE_CXX_STANDARD 17)

include_directories(~/local/include)


include_directories(src)
include_directories(${ROOT_INCLUDE_DIR})
include_directories(${EVTGEN_INCLUDE_DIR})

include_directories(./)
include_directories(src/)




#adding sources for rrF.exe
add_executable (rrF.exe "src/rrF.cc" "src/cut.cc" "src/RrfEvent.cpp" "src/RrfVar.cpp")
target_link_libraries(rrF.exe ${ROOT_LIBRARIES} ${EVTGEN_LIBRARY_DIR}/libEvtGen.a)

#adding sources for simpleEvtGenRO.exe
add_executable (simpleEvtGenRO.exe "src/simpleEvtGenRO.cc" "src/a.for")
target_link_libraries(simpleEvtGenRO.exe ${ROOT_LIBRARIES} ${EVTGEN_LIBRARY_DIR}/libEvtGen.a ${HEPMC_LIBRARY_DIR}/libHepMC3-static.a)

#adding sources for segR library
add_library(segR "src/segR.cc")
target_link_libraries(segR ${ROOT_LIBRARIES} ${EVTGEN_LIBRARY_DIR}/libEvtGen.a ${HEPMC_LIBRARY_DIR}/libHepMC3-static.a)


#adding sources for segR_test.exe
add_executable (segR_test.exe "src/segR_test.cc" "src/a.for"  "src/EvtProbeModel.cpp")
target_link_libraries(segR_test.exe  segR)
