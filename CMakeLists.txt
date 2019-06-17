MESSAGE("Hello, world")
IF(DEFINED APPLE)
    MESSAGE("APPLE")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ENDIF(DEFINED APPLE)

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

## set fortran flags
SET_COMPILE_FLAG(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS}"
                 Fortran "-fno-automatic")



include_directories(~/local/include)


include_directories(src)
include_directories(${ROOT_INCLUDE_DIR})
include_directories(${EVTGEN_INCLUDE_DIR})
include_directories(/usr/local/Cellar/boost/1.58.0/include/)

include_directories(./)
include_directories(src/)




#adding sources for rrF.exe
add_executable (rrF.exe "src/rrF.cc")
target_link_libraries(rrF.exe ${ROOT_LIBRARIES} ${EVTGEN_LIBRARY_DIR}/archive/libEvtGen.a)

#adding sources for rrF.exe
# add_executable (simpleEvtGenRo.exe "src/simpleEvtGenRo.cc")
# target_link_libraries(simpleEvtGenRo.exe ${ROOT_LIBRARIES} ${EVTGEN_LIBRARY_DIR}/archive/libEvtGen.a)
