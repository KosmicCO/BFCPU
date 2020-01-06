CXX=g++
CXXFLAGS=-Wall -Wextra -pedantic -Werror -std=c++17 -g
OPTFLAGS=-O3
LDFLAGS=$(CXXFLAGS) $(OPTFLAGS)
SRC=data_tape.cc peripheral_tape.cc bfcpu_interpreter.cc non_protocol_print.cc peripherals/keyboard_interface.cc
TESTS=data_tape_test peripheral_tape_test bfcpu_interpreter_test
TESTOBJ=data_tape_test.o peripheral_tape_test.o bfcpu_interpreter.o bfcpu_interpreter_test.o
OBJ=$(SRC:.cc=.o)

lib: $(OBJ)
	ar rcs libbfcpu_virtual_arch.a $(OBJ)

bfcpu_interpreter_test: bfcpu_interpreter_test.o $(OBJ)
	$(CXX) $(LDFLAGS) -o bfcpu_interpreter_test bfcpu_interpreter_test.o $(OBJ)

peripheral_tape_test: peripheral_tape_test.o $(OBJ)
	$(CXX) $(LDFLAGS) -o peripheral_tape_test peripheral_tape_test.o $(OBJ)

data_tape_test: data_tape_test.o $(OBJ)
	$(CXX) $(LDFLAGS) -o data_tape_test data_tape_test.o $(OBJ)

%.o.cc: %.cc %.hh
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJ) $(TESTS) $(TESTOBJ) bfcpu_virtual_arch.a 
