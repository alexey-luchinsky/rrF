GCC=g++

LIB_EVTGEN=/Users/luchinsky/Work/HEPforge/evtgen_build/lib/archive/libEvtGen.a
LIB_MC=/Users/luchinsky/Work/EvtGen/external/HepMC/lib/libHepMC.a
#ROOT_LIBS = `root-config --libs`
ROOT_LIBS = -L/usr/local/Cellar/root/6.12.04_1/lib/root -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -lpthread -stdlib=libc++ -lm -ldl
LIBS = ${ROOT_LIBS} ${LIB_EVTGEN} ${LIB_MC}

#ROOT_INCDIR = `root-config --incdir`
ROOT_INCDIR = /usr/local/Cellar/root/6.12.04_1/include/root
INC=-I ${ROOT_INCDIR}  -I../evtgen/ -I  ~/local/include/

#FLAGS = `root-config --cflags`
FLAGS = -pthread -stdlib=libc++ -std=c++11 -m64
all: kks_main.exe simpleEvtGenRO.exe readRootFile.exe rrF.exe


%.exe: %.cpp ${LIB_EVTGEN}
	${GCC} -o $@  $< ${FLAGS}  ${INC} ${LIBS}

%.exe: %.cc ${LIB_EVTGEN}
	${GCC} -o $@  $< ${FLAGS}  ${INC} ${LIBS}

clean:
	rm *.exe *~
