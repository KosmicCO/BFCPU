CXX=g++
CXXFLAGS=-Wall -lboost_program_options -Wextra -pedantic -Werror -std=c++17 -g
OPTFLAGS=-O3
LDFLAGS=$(CXXFLAGS) $(OPTFLAGS)
MEXE=bfasm
MOBJ=bfasm.o
SRC=
OBJ=$(SRC:.cc=.o)

bfasm: bfasm.o $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ bfasm.o $(OBJ)

%.o.cc: %.cc %.hh
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJ) $(MEXE) $(MOBJ)
