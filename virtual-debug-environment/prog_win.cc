#include "prog_win.hh"

ProgWin::ProgWin(const std::string file, int length, int width) {
    w_lines = length;
    w_cols = width;
    line = 0;
    sub_line = 0;
    scale_index = 0;

    cursor_index = 0;
    cursor_inline = 0;

    std::ifstream prog_stream;
    prog_stream.open(file);

    if(!prog_stream) {
        throw std::runtime_error("Could not open file.");
    }

    prog = std::string(std::istreambuf_iterator<char>(prog_stream), {});

    line_starts.push_back(0);

    for(unsigned int i = 0; i < prog.length(); i++) {
        if(prog[i] == '\n') {
            line_starts.push_back(i + 1);
        }
    }

    if(line_starts.back() != static_cast<int>(prog.length())){
        line_starts.push_back(prog.length());
    }
}

int ProgWin::get_start_index() {
    return line_starts[line] + sub_line * w_cols;
}

char ProgWin::get_char(int index) {
    if(index < 0 || index >= static_cast<int>(prog.length())) {
        throw std::runtime_error("Get character index is out of bounds.");
    }

    return prog[index];
}

bool ProgWin::pointed_to(int index, BFCPUInterpreter &bfi) {
    return index == bfi.get_prog_ptr();
}

int ProgWin::get_line_from_index(int index) {
    auto above_line = std::upper_bound(line_starts.begin(), line_starts.end(), index);
    return above_line - line_starts.begin() - 1;
}

int ProgWin::max_sub_lines(int of_line) {
    int c_num = line_starts[of_line + 1] - line_starts[of_line] - 1;
    if(c_num <= 0) {
        return 1;
    }
    return (c_num - 1) / w_cols + 1; // round up
}

int ProgWin::index_sub_line(int index) {
    return index / w_cols;
}

void ProgWin::line_down() {
    int msl = max_sub_lines(line);
    if(sub_line == msl - 1) {
        if(line < static_cast<int>(line_starts.size() - 1)) {
            line++;
            sub_line = 0;
        }
    } else {
        sub_line++;
    }

    scale_index = get_start_index();
}

void ProgWin::line_up() {
    if(sub_line == 0) {
        if(line > 0) {
            line--;
            sub_line = max_sub_lines(line) - 1;
        }
    } else {
        sub_line--;
    }

    scale_index = get_start_index();
}

void ProgWin::line_jump(int to_line) {
    if(to_line < 0 || to_line >= static_cast<int>(line_starts.size() - 1)) {
        throw std::runtime_error("The line to jump to is out of bounds.");
    }

    line = to_line;
    sub_line = 0;
    scale_index = get_start_index();
}

void ProgWin::index_jump(int index) {
    if(index < 0 || index >= static_cast<int>(prog.length())) {
        throw std::runtime_error("Index to jump to is out of bounds.");
    }
    line = get_line_from_index(index);
    sub_line = index_sub_line(index - line_starts[line]);
    scale_index = get_start_index();
}

void ProgWin::set_dim(int height, int width) {
    if(w_lines != height || w_cols != width) {
        w_lines = height;
        w_cols = width;
        int si_tmp = scale_index;
        index_jump(scale_index);
        scale_index = si_tmp;
    }
}

bool ProgWin::at_cursor(int index) {
    return index == cursor_index;
}

void ProgWin::move_cursor(int dir) {
    int at_line;
    switch(dir) {
        case DOWN:
            at_line = get_line_from_index(cursor_index);
            if(at_line < static_cast<int>(line_starts.size() - 1)) {
                cursor_index = line_starts[at_line + 1];
                int line_length = line_starts[at_line + 2] - cursor_index - 2;
                cursor_index += std::max(0, std::min(line_length, cursor_inline));
            }
            break;
        case UP:
            at_line = get_line_from_index(cursor_index);
            if(at_line > 0) {
                cursor_index = line_starts[at_line - 1];
                int line_length = line_starts[at_line] - cursor_index - 2;
                cursor_index += std::max(0, std::min(line_length, cursor_inline));
            }
            break;
        case LEFT:
            if(cursor_index > 0) {
                if(prog[cursor_index - 1] == '\n' && cursor_index - 2 >= 0 && prog[cursor_index - 2] != '\n') {
                    cursor_index -= 2;
                } else {
                    cursor_index--;
                }
                cursor_inline = cursor_index - line_starts[get_line_from_index(cursor_index)];
            }
            break;
        case RIGHT:
            if(cursor_index < static_cast<int>(prog.length())) {
                if(prog[cursor_index + 1] == '\n' && cursor_index + 2 <= static_cast<int>(prog.length()) && prog[cursor_index] != '\n') {
                    cursor_index += 2;
                } else {
                    cursor_index++;
                }
                cursor_inline = cursor_index - line_starts[get_line_from_index(cursor_index)];
            }
            break;
    }
}

void ProgWin::set_cursor(int to_line) {
    if(to_line >= static_cast<int>(line_starts.size())) {
        throw std::runtime_error("The line to set the cursor position is out of bounds.");
    }
    cursor_index = line_starts[to_line];
}

bool ProgWin::at_break(int index) {
    if(index < 0 || index >= static_cast<int>(prog.length())) {
        throw std::runtime_error("Index to poll break is out of bounds.");
    }
    return p_breaks.count(index) > 0;
}

bool ProgWin::at_break(BFCPUInterpreter &bfi) {
    return p_breaks.count(static_cast<int>(bfi.get_prog_ptr())) > 0;
}

void ProgWin::toggle_break_at_cursor() {
    if(is_bfcpu_char(prog[cursor_index])) {
        if(p_breaks.count(cursor_index)) {
            p_breaks.erase(cursor_index);
        } else {
            p_breaks.insert(cursor_index);
        }
    }
}








