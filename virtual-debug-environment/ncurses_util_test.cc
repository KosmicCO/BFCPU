#include "ncurses_util.hh"
#include <ncurses.h>
#include <iostream>

int main() {
    initscr();
    cbreak();
    
    keypad(stdscr, TRUE);

    char hw[21];
    getnstr_esc(hw, 20);

    mvprintw(1, 0, hw);
    refresh();
    getch();
    endwin();

    for(auto i = 0; i < 20; i++) {
        std::cout << static_cast<int>(hw[i]) << std::endl;
    }

    return 0;
}
