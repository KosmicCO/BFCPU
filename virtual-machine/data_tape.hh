#pragma once

#include "tape.hh"
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <stdexcept>

const int SIZE = 1 << 16;

class DataTape : public Tape {
    public:
        DataTape(uint16_t pages);
        ~DataTape();

        uint16_t read(uint16_t address);
        void write(uint16_t address, uint16_t value);
        void set_page(uint16_t s_page);
        uint16_t get_page();
        uint16_t get_num_pages();

        void get_data(uint16_t start, uint16_t end, uint16_t * arr);
        void get_data_from_page(uint16_t start, uint16_t end, uint16_t * arr, uint16_t from_page);
    private:
        uint16_t ** memory;
        uint16_t num_pages;
        uint16_t page;

        void touch_page(uint16_t t_page);
};
