include ../../paths.mk

OBJ      := MyModule1.o MyModule2.o MyModule3.o MyModule4.o MyModule5.o

#-----------------------------------------------------------
OBJ      += Using.o
CXXFLAGS += 

LDOPT    := -Xlinker -rpath -Xlinker `cat ../../.release`/../lib/

STDINC   := -std=c++11 -fPIC $(CXXFLAGS) -I./ -I../../ -I`cat ../../.release` `root-config --cflags` $(BOOSTFLAGS) $(CLHEPFLAGS)
STDLIB   := -lstdc++ -L../../lib -lAppFramework -lAnModBasic -lAnUtils -lAnObjects -lLogistics -L`cat ../../.release`/lib `root-config --glibs` -lGenVector

CXXFLAGS += $(STDINC)

all:	makefile $(OBJ)
	$(CXX) -shared $(LDOPT) $(OBJ) $(STDLIB) -o AppFramework.so
	@echo "done"

bin:	makefile $(OBJ)
	$(CXX) -o test.o -c $(CXXFLAGS) test.cc
	$(CXX) $(LDOPT) -o test $(OBJ) test.o $(STDLIB)
	@echo "done"

$(OBJ):		%.o: %.cc %.h
		$(CXX) -o $@ -c $(CXXFLAGS) $<
clean:
		@rm -f *.o test AppFramework.so init.pyc
