#pragma once

#include <iostream>
#include "data_tape.hh"
#include "peripheral_tape.hh"
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <stack>
#include <string>
#include <fstream>

const int ROM_SIZE = 1 << 20;
const int DEFAULT_NUM_PAGES = 1024;
const std::vector<Peripheral *> EMPTY_PV = std::vector<Peripheral *>();

bool is_bfcpu_char(char c);

class BFCPUInterpreter {
    public:
        BFCPUInterpreter(std::istream &rom, const std::vector<Peripheral *> &perphs=EMPTY_PV, int num_pages=DEFAULT_NUM_PAGES, bool strip=true);

        BFCPUInterpreter(const std::string &file, const std::vector<Peripheral *> &perphs=EMPTY_PV, int num_pages=DEFAULT_NUM_PAGES, bool strip=true);
        ~BFCPUInterpreter();

        bool next(bool comm=true); // returns false when cannot run
        
        uint16_t get_time();
        void set_time(uint16_t ntime);
        int get_num_pages();
        uint16_t get_page_ptr();
        uint16_t get_data_ptr();
        uint16_t get_prog_ptr();

        void get_tape_data(uint16_t start, uint16_t end, uint16_t * arr);
        void get_tape_data_from_page(uint16_t start, uint16_t end, uint16_t * arr, uint16_t from_page);
        
        void set_tape_at(uint16_t addr, uint16_t value);
        void set_tape_at_page(uint16_t addr, uint16_t page, uint16_t value);
    private:
        char * prog;
        int * jumps;
        int prog_len;
        int prog_ptr;
        uint16_t data_ptr;
        uint16_t buffer;
        DataTape * data_tape;
        PeripheralTape * perp_tape;

        void construct_sequence(std::istream &rom, const std::vector<Peripheral *> &perphs, int num_pages, bool strip);

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
