#pragma once

#include "../virtual-machine/bfcpu_virtual_arch.hh"
#include <stdexcept>
#include <algorithm>
#include "directions.hh"
#include <set>
#include <utility>

typedef std::pair<uint16_t, uint16_t> address_t;

class MemoryWin {
    public:
        MemoryWin(BFCPUInterpreter &bfi, int len=1, int cols=1);
        ~MemoryWin();

        void update(BFCPUInterpreter &bfi, int len);
        
        uint16_t get_tape_data(int index);
        bool pointed_to(int index);
        bool at_cursor(int index) { return index == cursor_index; }
        bool pointer_in_tape(BFCPUInterpreter &bfi);
        uint16_t get_offset_of_cursor() { return  tape_offset + static_cast<uint16_t>(cursor_index); }
        uint16_t get_offset() { return tape_offset; }
        uint16_t get_page() { return page_offset; }
        uint16_t get_length() { return tape_length; }

        void move_visual_cursor(int dir);
        void move_offset(int dir);
        void jump_to_ptr(BFCPUInterpreter &bfi);

        void set_offset(uint16_t tape_off, uint16_t page_off);
        void set_cols(int num_cols);
        bool in_visual_mode() { return visual_mode; }
        void set_visual_mode(bool vis) { visual_mode = vis; }

        bool at_break(int index);
        bool at_break(BFCPUInterpreter &bfi);
        void toggle_break_at_cursor();
        void clear_breaks() { t_breaks.clear(); }

        void set_tape_at(BFCPUInterpreter &bfi, uint16_t addr, uint16_t page, uint16_t value) { bfi.set_tape_at_page(addr, page, value); }
    private:
        uint16_t * tape_data;
        uint16_t tape_offset;
        uint16_t page_offset;
        int tape_length;

        uint16_t data_page_ptr;
        uint16_t data_tape_ptr;

        bool visual_mode;
        int cursor_index;
        int cols;
        std::set<std::pair<uint16_t, uint16_t>> t_breaks;

        void reinit(BFCPUInterpreter &bfi, int len);
};
