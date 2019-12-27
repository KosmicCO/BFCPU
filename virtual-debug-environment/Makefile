CXX=g++
CXXFLAGS=-Wall -lncurses -I../virtual-machine -L../virtual-machine -lbfcpu_virtual_arch -lboost_regex -Wextra -pedantic -Werror -std=c++17 -g
OPTFLAGS=-O3
LDFLAGS=$(CXXFLAGS) $(OPTFLAGS)
SRC=memory_win.cc command_win.cc command_win_colors.cc prog_win.cc ncurses_util.cc
OBJ=$(SRC:.cc=.o)

bfcpu_environment: bfcpu_environment.o $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ bfcpu_environment.o $(OBJ) -l bfcpu_virtual_arch

memory_win_test: memory_win_test.o $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ memory_win_test.o $(OBJ) -l bfcpu_virtual_arch

ncurses_util_test: ncurses_util_test.o $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ ncurses_util_test.o $(OBJ) -l bfcpu_virtual_arch

%.o.cc: %.cc %.hh
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJ) $(TESTS) $(TESTOBJ) memory_win_test bfcpu_environment ncurses_util_test memory_win_test.o bfcpu_environment.o ncurses_util_test.o
