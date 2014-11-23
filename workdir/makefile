#OBJ      := MyModule1.o MyModule2.o MyModule3.o MyModule4.o MyModule5.o
OBJ      :=

#-----------------------------------------------------------
OBJ      += Using.o
CXX	 := /cvmfs/cms.cern.ch/slc6_amd64_gcc481/external/gcc/4.8.1/bin/g++
CXXFLAGS := -std=c++11 -Wall -frtti -fexceptions -fpic -g

LDOPT    := -Xlinker -rpath -Xlinker `cat ../.release`/lib/

STDINC   := -I./ -I../../ -I`cat ../.release` `root-config --cflags` -I/cvmfs/cms.cern.ch/slc6_amd64_gcc481/cms/cmssw/CMSSW_7_2_0_pre8/src/ -I/cvmfs/cms.cern.ch/slc6_amd64_gcc481/external/clhep/2.1.4.1-cms/include/
STDLIB   := -lstdc++ -L../../lib -lAnUtils -lAppFramework -lAnUtils -lAnModBasic -lReaders -lAnObjects -L`cat ../.release`/lib `root-config --glibs` -lGenVector

STDLIB   += -L/cvmfs/cms.cern.ch/slc6_amd64_gcc481/cms/cmssw/CMSSW_7_2_0_pre8/lib/slc6_amd64_gcc481/ -lFWCoreUtilities -lDataFormatsPatCandidates -lDataFormatsTrackReco

CXXFLAGS += $(STDINC)

all:	makefile $(OBJ)
	$(CXX) -shared $(LDOPT) $(OBJ) $(STDLIB) -o AppFramework.so
	@echo "done"

bin:	makefile $(OBJ)
	$(CXX) -o main.o -c $(CXXFLAGS) main.cc
	$(CXX) $(LDOPT) -o test $(OBJ) main.o $(STDLIB)
	@echo "done"

$(OBJ):		%.o: %.cc %.h
		$(CXX) -o $@ -c $(CXXFLAGS) $<
clean:
		@rm -f *.o Makefile Makefile.old
