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
        CommandWin(const std::string &file, const std::vector<Peripheral *> &perphs=EMPTY_PV, int pages=DEFAULT_NUM_PAGES) : bfi(file, perphs, pages, false), mem_win(bfi, 1), prog_win(file, 1, 1), mid_split(0), bottom_text("") {};
        ~CommandWin() = default;

        bool next_comm();
    private:
        BFCPUInterpreter bfi;
        MemoryWin mem_win;
        ProgWin prog_win;

        int mid_split;
        std::string bottom_text;

        void update();
        void set_viewer_state();
        void render_mem(int lines, int cols);
        void render_prog(int lines, int cols);
        void render_mode();
        bool take_inline();
        bool take_inline(std::string seed);
        bool parse_comm(std::string comm);
        bool run_till_break(int cycles);

        // letter commands
        
        bool lv_set_at();

        // commands

        void cm_set(comm_t &);
        void cm_jp(comm_t &); // jump to line (prog_win)
        void cm_jv(comm_t &); // jump to line (mem_win)
        void cm_run(comm_t &);

        void undefined(comm_t &);
};
