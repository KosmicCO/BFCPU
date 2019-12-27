#include "../virtual-machine/bfcpu_virtual_arch.hh"
#include <iostream>
#include <ncurses.h>
#include "command_win.hh"
#include "command_win_colors.hh"

int main(int argc, char ** argv) {

    if(argc != 2) {
        std::cout << "Requires bf program as input." << std::endl;
        return 2;
    }

    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE); 
    ESCDELAY = 0;
    curs_set(0);

    init_command_win_colors();

    CommandWin com_win(argv[1]);

    while(com_win.next_comm());

    endwin();

    return 0;
}






