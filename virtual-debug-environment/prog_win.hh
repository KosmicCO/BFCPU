#pragma once

#include "../virtual-machine/bfcpu_virtual_arch.hh"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include "directions.hh"
#include <set>

class ProgWin {
    public:
        ProgWin(const std::string file, int length, int width);
        ~ProgWin() = default;

        int get_start_index();
        char get_char(int index);
        int get_max_index() { return static_cast<int>(prog.length()); }
        bool pointed_to(int index, BFCPUInterpreter &bfi);
        void jump_to_ptr(BFCPUInterpreter &bfi) { index_jump(bfi.get_prog_ptr()); }
        int get_line() { return line; }
        int get_num_lines() { return static_cast<int>(line_starts.size() - 1); }

        void line_down();
        void line_up();
        void line_jump(int to_line);
        void index_jump(int index);

        bool at_cursor(int index);
        void move_cursor(int dir);
        void set_cursor(int to_line);

        bool at_break(int index);
        bool at_break(BFCPUInterpreter &bfi);
        void toggle_break_at_cursor();
        void clear_breaks() { p_breaks.clear(); }

        void set_dim(int length, int width);
    private:
        std::vector<int> line_starts;
        std::string prog;
        int w_lines;
        int w_cols;
        int line;
        int sub_line;
        int scale_index;

        int cursor_index;
        int cursor_inline;
        std::set<int> p_breaks;

        int get_line_from_index(int index);
        int max_sub_lines(int of_line);
        int index_sub_line(int index);
};
