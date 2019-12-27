#include "command_win_colors.hh"

void init_command_win_colors() {
    if(!has_colors()) {
        throw std::runtime_error("Terminal does not support colors.");
    }

    start_color();

    init_pair(PAIR_LABEL_GRAY, 8, COLOR_BLACK);
    init_pair(PAIR_LABEL_GREEN, 48, COLOR_BLACK);
    init_pair(PAIR_LABEL_ORANGE, 214, COLOR_BLACK);

    init_pair(PAIR_HIGHLIGHT_GREEN, COLOR_WHITE, 28);
    init_pair(PAIR_HIGHLIGHT_WHITE, COLOR_BLACK, COLOR_WHITE);
    init_pair(PAIR_HIGHLIGHT_LIGHT_GREEN, COLOR_BLACK, 46);
}
