#include "bfcpu_interpreter.hh"
#include <iostream>
#include <fstream>
#include <vector>
#include "non_protocol_print.hh"

int main(int argc, char ** argv) {
    if(argc != 2) {
        std::cout << "Only takes file to load as arguments." << std::endl;
        return 2;
    }
    std::ifstream rom_prog;
    rom_prog.open(argv[1]);
    if(!rom_prog) {
        std::cout << "Could not open file." << std::endl;
        return 3;
    }

    std::istream &rom_stream = rom_prog;
    std::vector<Peripheral *> perphs;
    perphs.push_back(new NonProtocolPrint());
    BFCPUInterpreter bfi(rom_stream, perphs);
    rom_prog.close();

    while(bfi.next());
    
    for(Peripheral * p : perphs) {
        if(p) {
            delete p;
        }
    }

    std::cout << std::endl << "Program completed." << std::endl;
    return 0;
}
