#pragma once

#include <iostream>
#include "data_tape.hh"
#include "peripheral_tape.hh"
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <stack>

const int ROM_SIZE = 1 << 20;
const int DEFAULT_NUM_PAGES = 1024;
const std::vector<Peripheral *> EMPTY_PV = std::vector<Peripheral *>();

class BFCPUInterpreter {
    public:
        BFCPUInterpreter(std::istream &rom, const std::vector<Peripheral *> &perphs, int num_pages);
        BFCPUInterpreter(std::istream &rom, const std::vector<Peripheral *> &perphs) : BFCPUInterpreter(rom, perphs, DEFAULT_NUM_PAGES) {}
        BFCPUInterpreter(std::istream &rom) : BFCPUInterpreter(rom, EMPTY_PV) {}
        ~BFCPUInterpreter();

        bool next(); // returns false when cannot run
        
        uint16_t get_time();
        void set_time(uint16_t ntime);
        uint16_t get_num_pages();
        uint16_t get_page_ptr();
        uint16_t get_data_ptr();

        void get_tape_data(uint16_t start, uint16_t end, uint16_t * arr);
    private:
        char * prog;
        int * jumps;
        int prog_len;
        int prog_ptr;
        uint16_t data_ptr;
        uint16_t buffer;
        DataTape * data_tape;
        PeripheralTape * perp_tape;

        uint16_t clock_time;
        
        uint16_t at_ptr();
        void pinc(); // increment program counter

        void move_left();
        void move_right();
        void increment();
        void decrement();
        void write();
        void read();
        void loop_back();
        void loop_forward();

        void jump_data();
        void copy();
        void paste();
        void left_shift();
        void nand();
        void jump_page(); // NOTE: does nothing on invalid page input
        void page_left();
        void page_right();
};
