include ../paths.mk

OBJ      := Analyser.o

#-----------------------------------------------------------
OBJ      += Using.o
CXXFLAGS += 

LDOPT    := -Xlinker -rpath -Xlinker `cat ../.release`/lib/

STDINC   := $(CXXFLAGS) -I./ -I../../ -I`cat ../.release` `root-config --cflags` $(BOOSTFLAGS) $(CLHEPFLAGS)
STDLIB   := -lstdc++ -L../../lib -lAppFramework -lAnModBasic -lAnUtils -lReaders -lAnObjects -lLogistics -L`cat ../.release`/lib `root-config --glibs` -lGenVector

STDLIB   += -L$(ROOT)/$(ARCH)/cms/cmssw/$(CMSSW)/lib/$(ARCH)/ -lFWCoreUtilities -lDataFormatsPatCandidates -lDataFormatsTrackReco -lDataFormatsCommon -lDataFormatsMuonReco -lDataFormatsL1TMuon -L../../../lib/$(ARCH)/

STDLIB   += -lDataFormatsProvenance -lDataFormatsHepMCCandidate -lFWCoreParameterSet
# -lProductAreaBNcollections -lDataFormatsCommon

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
