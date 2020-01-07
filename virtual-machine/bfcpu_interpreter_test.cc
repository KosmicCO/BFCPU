#include "bfcpu_interpreter.hh"
#include <iostream>
#include <fstream>
#include <vector>
#include "non_protocol_print.hh"
#include "peripheral.hh"
#include "peripherals/ncurses_keyboard_terminal.hh"
#include <chrono>

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

    std::string s;
    std::cin >> s;

    NCursesKeyboardTerminal * term = NCursesKeyboardTerminal::get_instance();
    std::shared_ptr<KeyboardInterface> key = term->get_keyboard_peripheral();

    std::istream &rom_stream = rom_prog;
    std::vector<Peripheral *> perphs;
    perphs.push_back(term);
    perphs.push_back(key.get());
    BFCPUInterpreter bfi(rom_stream, perphs, 1948);
    rom_prog.close();

    auto last_time = std::chrono::system_clock::now();
    auto cur_time = last_time;
    term->render();
    const std::chrono::duration<double, std::ratio<1, 30>> frame_time(1);

    while(bfi.next()) {
        cur_time = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = cur_time - last_time;
        if(diff >= frame_time) {
            term->render();
            last_time = cur_time;
        }
    };
    
    /*
    for(Peripheral * p : perphs) {
        if(p) {
            delete p;
        }
    }
    */

    perphs.clear();

    std::cout << std::endl << "Program completed." << std::endl;
    return 0;
}
