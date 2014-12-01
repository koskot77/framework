include ../paths.mk

#OBJ      := MyModule1.o MyModule2.o MyModule3.o MyModule4.o MyModule5.o
OBJ      := Analyser.o

#-----------------------------------------------------------
OBJ      += Using.o
CXXFLAGS += 

LDOPT    := -Xlinker -rpath -Xlinker `cat ../.release`/lib/

STDINC   := $(CXXFLAGS) -I./ -I../../ -I`cat ../.release` `root-config --cflags`
STDLIB   := -lstdc++ -L../../lib -lAppFramework -lAnModBasic -lAnUtils -lReaders -lAnObjects -L`cat ../.release`/lib `root-config --glibs` -lGenVector

STDLIB   += -L$(ROOT)/$(ARCH)/cms/cmssw/$(CMSSW)/lib/$(ARCH)/ -lFWCoreUtilities -lDataFormatsPatCandidates -lDataFormatsTrackReco -lDataFormatsCommon -lDataFormatsMuonReco -L../../../lib/$(ARCH)/ -lProductAreaBNcollections -lDataFormatsCommon

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
