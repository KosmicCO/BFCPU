#pragma once

#include "tape.hh"
#include <algorithm>
#include <exception>

const int SIZE = 1 << 16;

class DataTape : public Tape {
    public:
        DataTape(uint16_t pages);
        ~DataTape();

        uint16_t read(uint16_t address);
        void write(uint16_t address, uint16_t value);
        void set_page(uint16_t page);
        uint16_t get_page();
        uint16_t get_num_pages();
    private:
        uint16_t ** memory;
        uint16_t num_pages;
        uint16_t page;
};
