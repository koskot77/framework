OBJ      := MyModule1.o MyModule2.o MyModule3.o MyModule4.o MyModule5.o

#-----------------------------------------------------------
OBJ      += Using.o
CXX	 := g++
CXXFLAGS := -std=c++11 -Wall -frtti -fexceptions -fpic -g

LDOPT    := -Xlinker -rpath -Xlinker `cat ../.release`/lib/

STDINC   := -I./ -I../../ -I`cat ../.release`
STDLIB   := -lstdc++ -L../../lib -lAnUtils -lAppFramework -lAnUtils -L`cat ../.release`/lib

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
