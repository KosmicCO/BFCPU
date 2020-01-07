#pragma once

#include "../peripheral.hh"
#include <mutex>
#include "keyboard_interface.hh"
#include "terminal_codes.hh"
#include <string>
#include <ncurses.h>
#include <algorithm>
#include <list>
#include <memory>
#include <thread>

class NCursesKeyboardTerminal : public Peripheral {
    public:
        ~NCursesKeyboardTerminal();
        static NCursesKeyboardTerminal * get_instance();

        void write(uint16_t value);
        uint16_t read();

        std::shared_ptr<KeyboardInterface> get_keyboard_peripheral() { return keyboard; }
        void render();
    private:
        NCursesKeyboardTerminal();
        static NCursesKeyboardTerminal * instance;

        HardwareInfo terminal_hardware_data;
        std::shared_ptr<HardwareInfo> keyboard_hardware_info;
        std::shared_ptr<KeyboardInterface> keyboard;

        bool running;
        std::thread * run_keyboard;
        std::list<std::list<wchar_t> *> lines;

        std::queue<uint16_t> hardware_out;
        std::queue<uint16_t> term_out;
        int last_comm;
        int reading;

        void ready_term_out();
        void print_char(uint16_t c);
};

