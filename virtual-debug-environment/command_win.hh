#pragma once

#include "../virtual-machine/bfcpu_virtual_arch.hh"
#include <string>
#include "memory_win.hh"
#include <ncurses.h>
#include "command_win_colors.hh"
#include "prog_win.hh"
#include <cmath>
#include "ncurses_util.hh"
#include <boost/algorithm/string.hpp>
#include <iomanip>

typedef std::vector<std::string> comm_t;

class CommandWin {
    public:
        CommandWin(const std::string &file, const std::vector<Peripheral *> &perphs, int pages) : bfi(file, perphs, pages, false), mem_win(bfi, 1), prog_win(file, 1, 1), mid_split(0) {};
        CommandWin(const std::string &file) : bfi(file, EMPTY_PV, DEFAULT_NUM_PAGES, false), mem_win(bfi, 1), prog_win(file, 1, 1), mid_split(0) {};
        ~CommandWin() = default;

        bool next_comm();
    private:
        BFCPUInterpreter bfi;
        MemoryWin mem_win;
        ProgWin prog_win;

        int mid_split;

        void update();
        void render_mem(int lines, int cols);
        void render_prog(int lines, int cols);
        bool take_inline();
        bool take_inline(std::string seed);
        bool parse_comm(std::string comm);

        // letter commands
        
        bool lv_set_at();

        // commands

        void cm_set(comm_t &);

        void undefined(comm_t &);
};
