#include "ncurses_util.hh"

void shift_right(char * str, int index) {
    if(index >= 0) {
        int start_index = index;
        char c = str[start_index];
        while(c != '\0') {
            start_index++;
            c = str[start_index];
        }
        str[start_index + 1] = '\0';
        for(; index < start_index; start_index--) {
            str[start_index] = str[start_index - 1];
        }
    }
}

void shift_left(char * str, int index) {
    char c;
    int count = index;
    do {
        c = str[count + 1];
        str[count] = c;;
        count++;
    } while (c != '\0');
}

void backspace(char * str, int &len, int &cursor) {
    if(cursor > 0 && cursor <= len) {
        shift_left(str, cursor - 1);
        cursor--;
        len--;
    }
}

void delete_char(char * str, int &len, int index) {
    if(index >= 0 && index < len) {
        shift_left(str, index);
        len--;
    }
}

void add_char(char * str, int n, int &len, int &cursor, char c) {
    if(len < n && cursor <= len && cursor >= 0) {
        shift_right(str, cursor);
        str[cursor] = c;
        cursor++;
        len++;
    }
}

int getnstr_esc(char * str, int n) {
    std::fill(str, str + n, '\0');
    return getnstr_esc_seed(str, n);
}

int getnstr_esc_seed(char * str, int n) {
    noecho();

    int y, x;
    getyx(stdscr, y, x);

    n = std::min(n, (LINES * COLS) - (COLS * y + x) - 1);

    int c;
    int len = std::min(static_cast<int>(strlen(str)), n);
    int cursor = len;
    str[len + 1] = '\0';

    while(true) {

        mvprintw(y, x, str);
        printw(" ");
        int wrap_y = y + ((x + cursor) / COLS);
        int wrap_x = (x + cursor) % COLS;
        move(wrap_y, wrap_x);
        refresh();

        c = getch();
        switch(c) {
            case KEY_ENTER:
            case '\n':
            case '\r':
                str[len] = '\0';
                return OK;
            case 27:
                std::fill(str, str + n, '\0');
                return OK;
            case 127:
            case KEY_BACKSPACE:
                backspace(str, len, cursor);
                break;
            case KEY_DC:
                delete_char(str, len, cursor); 
                break;
            case KEY_LEFT:
                if(cursor > 0) {
                    cursor--;
                }
                break;
            case KEY_RIGHT:
                if(cursor < len) {
                    cursor++;
                }
                break;
            default:
                if((c & 0xFFFFFF00) == 0 && c >= 32) {
                    add_char(str, n, len, cursor, c);
                }
        }
    }
}
