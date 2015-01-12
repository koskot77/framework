include ../paths.mk

OBJ      := MyModule1.o MyModule2.o MyModule3.o MyModule4.o MyModule5.o

#-----------------------------------------------------------
OBJ      += Using.o
CXXFLAGS += 

LDOPT    := -Xlinker -rpath -Xlinker `cat ../../.release`/lib/

STDINC   := -std=c++11 -fPIC $(CXXFLAGS) -I./ -I../../ -I`cat ../../.release` `root-config --cflags` $(BOOSTFLAGS) $(CLHEPFLAGS)
STDLIB   := -lstdc++ -L../../lib -lAppFramework -lAnModBasic -lAnUtils -lReaders -lAnObjects -lLogistics -L`cat ../../.release`/lib `root-config --glibs` -lGenVector

CXXFLAGS += $(STDINC)

all:	makefile $(OBJ)
	$(CXX) -shared $(LDOPT) $(OBJ) $(STDLIB) -o AppFramework.so
	@echo "done"

bin:	makefile $(OBJ)
	$(CXX) -o qwe.o -c $(CXXFLAGS) qwe.cc
	$(CXX) $(LDOPT) -o test $(OBJ) qwe.o $(STDLIB)
	@echo "done"

$(OBJ):		%.o: %.cc %.h
		$(CXX) -o $@ -c $(CXXFLAGS) $<
clean:
		@rm -f *.o Makefile Makefile.old
