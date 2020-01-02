#include "command_win.hh"

void CommandWin::update() {
    clear();
    if(LINES < 15 || COLS < 15) {
        mvprintw(0, 0, "Window Too Small.");
    } else {
        int split = (LINES / 2) + mid_split; 

        if(split - 1 < 4) {
            mid_split = 5 - (LINES / 2);
            split = (LINES / 2) + mid_split;
        } else if(LINES - split < 4) {
            mid_split = (LINES / 2) - 4;
            split = (LINES / 2) + mid_split;
        }

        mvhline(split - 1, 0, ACS_HLINE, COLS);
        render_mem(split - 1, COLS);
        render_prog(LINES - split, COLS);
        mvprintw(LINES - 1, 0, bottom_text.c_str());
        render_mode();
    }
    refresh();
}

void CommandWin::set_viewer_state() {
    bottom_text = "";
}

void CommandWin::render_mem(int length, int width) {

    const int cols = (width - 2)/ 6;
    const int rows = length - 2;

    mem_win.set_cols(cols - 1);
    mem_win.update(bfi, (cols - 1) * (rows - 1));

    attron(COLOR_PAIR(PAIR_LABEL_GREEN));
    mvprintw(1, 1, "%04x", mem_win.get_page());
    attroff(COLOR_PAIR(PAIR_LABEL_GREEN));

    attron(COLOR_PAIR(PAIR_LABEL_GRAY));
    for(int i = 1; i < cols; i++) {
        mvprintw(1, i * 6 + 1, " %04x", i - 1);
    }
    for(int i = 1; i < rows; i++) {
        mvprintw(i + 1, 1, "%04x", ((i - 1) * (cols - 1) + mem_win.get_offset()) % 0x10000);
    }
    attroff(COLOR_PAIR(PAIR_LABEL_GRAY));

    int index = 0;
    int cur = 0;
    for(int i = 1; i < rows; i++) {
        for(int j = 1; j < cols; j++) {
            cur = 0;
            cur |= mem_win.pointed_to(index) ? 1 : 0;
            cur |= mem_win.at_cursor(index) ? 2 : 0;
            cur |= mem_win.at_break(index) ? 4 : 0;

            switch(cur) {
                case 0b001: attron(COLOR_PAIR(PAIR_HIGHLIGHT_WHITE));           break;
                case 0b010: attron(COLOR_PAIR(PAIR_HIGHLIGHT_GREEN));           break;
                case 0b011: attron(COLOR_PAIR(PAIR_HIGHLIGHT_LIGHT_GREEN));     break;
                case 0b100: attron(COLOR_PAIR(PAIR_HIGHLIGHT_DEEP_RED));        break;
                case 0b101: attron(COLOR_PAIR(PAIR_HIGHLIGHT_YELLOW));          break;
                case 0b110: attron(COLOR_PAIR(PAIR_HIGHLIGHT_RED));             break;
                case 0b111: attron(COLOR_PAIR(PAIR_HIGHLIGHT_ORANGE));          break;
            }

            mvprintw(i + 1, j * 6 + 1, " %4x", mem_win.get_tape_data(index));

            switch(cur) {
                case 0b001: attroff(COLOR_PAIR(PAIR_HIGHLIGHT_WHITE));           break;
                case 0b010: attroff(COLOR_PAIR(PAIR_HIGHLIGHT_GREEN));           break;
                case 0b011: attroff(COLOR_PAIR(PAIR_HIGHLIGHT_LIGHT_GREEN));     break;
                case 0b100: attroff(COLOR_PAIR(PAIR_HIGHLIGHT_DEEP_RED));        break;
                case 0b101: attroff(COLOR_PAIR(PAIR_HIGHLIGHT_YELLOW));          break;
                case 0b110: attroff(COLOR_PAIR(PAIR_HIGHLIGHT_RED));             break;
                case 0b111: attroff(COLOR_PAIR(PAIR_HIGHLIGHT_ORANGE));          break;
            }

            index++;
        }
    }
}

void render_line_num(int max_len, int num) {
    char fstr[65];
    sprintf(fstr, "%%%dd ", max_len);
    attron(COLOR_PAIR(PAIR_LABEL_ORANGE));
    printw(fstr, num);
    attroff(COLOR_PAIR(PAIR_LABEL_ORANGE));
}

void CommandWin::render_prog(int length, int width) {

    const int num_buff = static_cast<int>(log10(prog_win.get_num_lines())) + 1;

    int y_start = LINES - length + 1;
    int cols = width - 2 - num_buff - 1;
    int rows = length - 2;

    int x, y = 0;

    int rows_left = rows - 1;
    int cols_left = cols;
    int index = prog_win.get_start_index();
    int cur_line = prog_win.get_line();

    const int max_index = prog_win.get_max_index();
    prog_win.set_dim(rows, cols);

    move(y_start, 1);
    render_line_num(num_buff, cur_line);

    while(rows_left > 0 && index < max_index) {
        char next = prog_win.get_char(index);
        if(next == '\n') {
            if(prog_win.at_cursor(index)) {
                addch(' ' | COLOR_PAIR(PAIR_HIGHLIGHT_GREEN));
            }
            getyx(stdscr, y, x);
            move(y + 1, 1);
            rows_left--;
            cols_left = cols;
            cur_line++;

            if(rows_left > 0) {
                render_line_num(num_buff, cur_line);
            }
        } else {
            int flags = prog_win.pointed_to(index, bfi) ? 1 : 0;
            flags |= prog_win.at_cursor(index) ? 2 : 0;
            flags |= prog_win.at_break(index) ? 4 : 0;

            int mod;

            switch(flags) {
                case 0b000: mod = A_NORMAL;                                 break;
                case 0b001: mod = COLOR_PAIR(PAIR_HIGHLIGHT_WHITE);         break;
                case 0b010: mod = COLOR_PAIR(PAIR_HIGHLIGHT_GREEN);         break;
                case 0b011: mod = COLOR_PAIR(PAIR_HIGHLIGHT_LIGHT_GREEN);   break;
                case 0b100: mod = COLOR_PAIR(PAIR_HIGHLIGHT_DEEP_RED);      break;
                case 0b101: mod = COLOR_PAIR(PAIR_HIGHLIGHT_YELLOW);        break;
                case 0b110: mod = COLOR_PAIR(PAIR_HIGHLIGHT_RED);           break;
                case 0b111: mod = COLOR_PAIR(PAIR_HIGHLIGHT_ORANGE);        break;
            }

            addch(next | mod);
            cols_left--;
            if(cols_left <= 0) {
                getyx(stdscr, y, x);
                move(y + 1, num_buff + 2);
                rows_left--;
                cols_left = cols;
            }
        }
        index++;
    }

    if(index == max_index && prog_win.at_cursor(index)) {
        addch(' ' | COLOR_PAIR(PAIR_HIGHLIGHT_GREEN));
    }

}

void CommandWin::render_mode() {
    if(mem_win.in_visual_mode()) {
        std::string mode = "[VISUAL]";
        mvprintw(LINES - 1, COLS - mode.length() - 1, mode.c_str());
    }
}

void split_comm(comm_t &tokens, const std::string str) {
    tokens.clear();
    boost::split(tokens, str, boost::is_any_of(" "));
    std::vector<std::string>::iterator it = remove_if(tokens.begin(), tokens.end(), std::mem_fun_ref(&std::string::empty));
    tokens.erase(it, tokens.end());
}

enum comm_code {
    com_q,      // quit
    com_set,    // sets cell of mem
    com_jp,     // jumps to line in prog
    com_jv,     // jumps to addr in mem
    com_run,    // run until break
    com_p,      // jump to page
    undef
};

comm_code get_comm_code(std::string head) {
    if(head == "q")         return com_q;
    if(head == "set")       return com_set;
    if(head == "jp")        return com_jp;
    if(head == "jv")        return com_jv;
    if(head == "run")       return com_run;
    if(head == "p")         return com_p;
    return undef;
}

bool CommandWin::parse_comm(std::string comm) {

    if(comm.empty()){
        return true;
    }

    comm_t tokens;
    split_comm(tokens, comm);

    switch(get_comm_code(tokens[0])){
        case com_q:     return false;
        case com_set:   cm_set(tokens);     break;
        case com_jp:    cm_jp(tokens);      break;
        case com_jv:    cm_jv(tokens);      break;
        case com_run:   cm_run(tokens);     break;
        case com_p:     cm_p(tokens);       break;
        case undef: undefined(tokens);
    }

    return true;
}

void CommandWin::cm_set(comm_t &tokens) {
    comm_t flags;
    for(std::string s : tokens) {
        if(s.length() >= 2 && s[0] == '-' && !isdigit(s[1])) {
            flags.push_back(s);
        }
    }

    tokens.erase(std::remove_if(tokens.begin(), tokens.end(), [] (const std::string s) { return s.length() >= 2 && s[0] == '-' && !isdigit(s[1]); }), tokens.end());

    if(tokens.size() < 4) {
        bottom_text = "Takes a minimum of 3 arguments";
        return;
    }

    int d_flag = 0; // 0: normal, 1: hex, 2: string
    for(std::string flag : flags) {
        if(flag == "-h") {
            d_flag = 1;
        } else {
            bottom_text = "Not a valid flag: " + flag;
            return;
        }
    }

    try {

        uint16_t addr = static_cast<uint16_t>(stoi(tokens[1], nullptr, 0) & 0xFFFF);
        const uint16_t page = static_cast<uint16_t>(stoi(tokens[2], nullptr, 0) & 0xFFFF);

        switch(d_flag) {
            case 0:
                for(unsigned long int i = 3; i < tokens.size(); i++) {
                    mem_win.set_tape_at(bfi, addr, page, static_cast<uint16_t>(stoi(tokens[i], nullptr, 0) & 0xFFFF));
                    addr++;
                }
                break;
            case 1:
                for(unsigned long int i = 3; i < tokens.size(); i++) {
                    mem_win.set_tape_at(bfi, addr, page, static_cast<uint16_t>(stoi(tokens[i], nullptr, 16) & 0xFFFF));
                    addr++;
                }
                break;
        }
    } catch(...) {
        bottom_text = "Error parsing values";
        return;
    }
}

void CommandWin::cm_jp(comm_t &tokens) {
    if(tokens.size() == 1) {
        prog_win.set_cursor(prog_win.get_line());
        return;
    }
    try {
        int line = stoi(tokens[1], nullptr, 0);
        if(line < 0 || line >= prog_win.get_num_lines()) {
            bottom_text = "Line number out of bounds";
        }
        prog_win.line_jump(line);
        prog_win.set_cursor(line);
    } catch(...) {
        bottom_text = "Error parsing value";
    }
}

void CommandWin::cm_jv(comm_t &tokens) {
    if(1 == tokens.size()) {
        mem_win.jump_to_ptr(bfi);
        return;
    }
    try {
        uint16_t addr = static_cast<uint16_t>(stoi(tokens[1], nullptr, 0) & 0xFFFF);
        if(static_cast<int>(tokens.size()) == 2) {
            mem_win.set_offset(addr, mem_win.get_page());
        } else {
            uint16_t page = static_cast<uint16_t>(stoi(tokens[2], nullptr, 0) & 0xFFFF);
            mem_win.set_offset(addr, page);
        }
    } catch(...) {
        bottom_text = "Error parsing value";
    }
}

bool CommandWin::run_till_break(int cycles) {
    bool no_stop = cycles == -1;
    bool running = true;
    while((no_stop || cycles > 0) && running) {
        running = bfi.next();
        if(prog_win.at_break(bfi) || mem_win.at_break(bfi)) {
            return running;
        }
        cycles--;
    }
    return running;
}

void CommandWin::cm_run(comm_t &tokens) {
    int cycles = -1;
    try {
        if(tokens.size() == 2) {
            cycles = stoi(tokens[1], nullptr, 0);
            if(cycles < -2) {
                bottom_text = "Invalid number of run cycles inputed: " + std::to_string(cycles);
                return;
            }
        }

        run_till_break(-1);
        if(!mem_win.pointer_in_tape(bfi)) {
            mem_win.jump_to_ptr(bfi);
        }
        prog_win.jump_to_ptr(bfi);
    } catch (...) {
        bottom_text = "Error parsing value";
        return;
    }
}

void CommandWin::cm_p(comm_t &tokens) {
    if(tokens.size() == 1) {
        mem_win.set_offset(mem_win.get_offset(), bfi.get_page_ptr());
        return;
    }
    if(static_cast<int>(tokens.size()) > 2) {
        bottom_text = "Too many arguments";
        return;
    }
    try {
        int pg = stoi(tokens[1], nullptr, 0);
        if(pg < 0 || pg >= bfi.get_num_pages()) {
            bottom_text = "Page not in bounds";
            return;
        }
        mem_win.set_offset(mem_win.get_offset(), static_cast<uint16_t>(pg));
    } catch (...) {
        bottom_text = "Error parsing page";
        return;
    }
}

void CommandWin::undefined(comm_t &tokens) {
    bottom_text = "Command not found: " + tokens[0];
}

bool CommandWin::take_inline() {
    return take_inline("");
}

bool CommandWin::take_inline(std::string seed) {
    move(LINES - 1, 0);
    clrtoeol();
    render_mode();
    move(LINES - 1, 0);
    addch(':');
    curs_set(1);

    char * comm = new char[std::max(257, static_cast<int>(seed.length() + 1))];
    strcpy(comm, seed.c_str());
    getnstr_esc_seed(comm, 256);

    bool ret = parse_comm(std::string(comm));
    delete[] comm;

    move(LINES - 1, 0);
    clrtoeol();
    curs_set(0);
    return ret;
}

bool CommandWin::next_comm() {
    update();
    int comm = getch();
    bool ret = true;
    set_viewer_state();
    if(mem_win.in_visual_mode()) {
        switch(comm) {
            case 27:
            case 'v': mem_win.set_visual_mode(false);           break;
            case 'h':
            case KEY_LEFT: mem_win.move_visual_cursor(LEFT);    break;
            case 'j':
            case KEY_DOWN: mem_win.move_visual_cursor(DOWN);    break;
            case 'k':
            case KEY_UP: mem_win.move_visual_cursor(UP);        break;
            case 'l':
            case KEY_RIGHT: mem_win.move_visual_cursor(RIGHT);  break;
            case 'y': mem_win.move_offset(LEFT);                break;
            case 'u': mem_win.move_offset(DOWN);                break;
            case 'i': mem_win.move_offset(UP);                  break;
            case 'o': mem_win.move_offset(RIGHT);               break;
            case KEY_NPAGE:
            case ';':
            case 'I': prog_win.line_down();                     break;
            case KEY_PPAGE:
            case '\'':
            case 'U': prog_win.line_up();                       break;
            case 'b': mem_win.toggle_break_at_cursor();         break;
            case 't': mem_win.jump_to_ptr(bfi);                 break;        
            case '{': lv_inc_page(UP);                          break;
            case '}': lv_inc_page(DOWN);                        break;
            case 'p': ret = lv_set_at();                        break;
            case 'g': ret = take_inline("jv ");                 break;
            case 'r': ret = take_inline("run ");                break;
            case ':': ret = take_inline();                      break;
        }
    } else {
        switch(comm) {
            case 'v': mem_win.set_visual_mode(true);        break;
            case 'D': mid_split++;                          break;
            case 'E': mid_split--;                          break;
            case 's': bfi.next();                           break;
            case KEY_LEFT:
            case 'h': prog_win.move_cursor(LEFT);           break;
            case KEY_DOWN:
            case 'j': prog_win.move_cursor(DOWN);           break;
            case KEY_UP:
            case 'k': prog_win.move_cursor(UP);             break;
            case KEY_RIGHT:
            case 'l': prog_win.move_cursor(RIGHT);          break;
            case KEY_NPAGE:
            case 'd': prog_win.line_down();                 break;
            case KEY_PPAGE:
            case 'e': prog_win.line_up();                   break;
            case '{': lv_inc_page(UP);                      break;
            case '}': lv_inc_page(DOWN);                    break;
            case 'y': mem_win.move_offset(LEFT);            break;
            case 'u': mem_win.move_offset(DOWN);            break;
            case 'i': mem_win.move_offset(UP);              break;
            case 'o': mem_win.move_offset(RIGHT);           break;
            case 'b': prog_win.toggle_break_at_cursor();    break;
            case 't': prog_win.jump_to_ptr(bfi);            break;
            case 'g': ret = take_inline("jp ");             break;
            case 'r': ret = take_inline("run ");            break;
            case ':': ret = take_inline();                  break;
        }
    }
    return ret;
}

bool CommandWin::lv_set_at() {
    std::stringstream addr;
    std::stringstream page;

    addr << std::hex << mem_win.get_offset_of_cursor();
    page << std::hex << mem_win.get_page();

    return take_inline("set 0x" + addr.str() + " 0x" + page.str() + " ");
}

void CommandWin::lv_inc_page(int dir) {
    uint16_t pg;
    switch(dir) {
        case DOWN:
            pg = mem_win.get_page();
            if(bfi.get_num_pages() - 1 <= pg) {
                pg = 0;
            } else {
                pg++;
            }
            mem_win.set_offset(mem_win.get_offset(), pg);
            break;
        case UP:
            pg = mem_win.get_page();
            if(pg == 0) {
                pg = bfi.get_num_pages() - 1;
            } else {
                pg--;
            }
            mem_win.set_offset(mem_win.get_offset(), pg);
            break;
    }
}









