#pragma once

#include <ncurses.h>
#include <stdexcept>

const int PAIR_LABEL_GRAY = 1;
const int PAIR_LABEL_GREEN = 2;
const int PAIR_LABEL_ORANGE = 3;

const int PAIR_HIGHLIGHT_GREEN = 4;
const int PAIR_HIGHLIGHT_WHITE = 5;
const int PAIR_HIGHLIGHT_LIGHT_GREEN = 6;
const int PAIR_HIGHLIGHT_YELLOW = 7;
const int PAIR_HIGHLIGHT_ORANGE = 8;
const int PAIR_HIGHLIGHT_RED = 9;
const int PAIR_HIGHLIGHT_DEEP_RED = 10;

void init_command_win_colors();
