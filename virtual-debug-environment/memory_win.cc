#include "memory_win.hh"
#include <iostream>

void MemoryWin::reinit(BFCPUInterpreter &bfi, int len) {
    if(len <= 0) {
        throw std::runtime_error("The length of the tape data must be greater than zero.");
    }
    if(tape_data) {
        delete[] tape_data; // NOTE: make sure tape_data is not deleted prior
        tape_data = nullptr;
    }

    data_page_ptr = bfi.get_page_ptr();
    data_tape_ptr = bfi.get_data_ptr();

    tape_length = len; 
    tape_data = new uint16_t[tape_length];
    bfi.get_tape_data(tape_offset, tape_offset + tape_length, tape_data);

    if(cursor_index >= tape_length) {
        cursor_index = tape_length - 1;
    }
}

MemoryWin::MemoryWin(BFCPUInterpreter &bfi, int len, int cols) {
    if(cols < 1) {
        throw std::runtime_error("Number of columns cannot be less than 1.");
    }

    tape_data = nullptr; 
    tape_offset = 0;
    page_offset = 0;

    visual_mode = false;
    cursor_index = 0;
    reinit(bfi, len);
}

MemoryWin::~MemoryWin() {
    delete [] tape_data;
}

void MemoryWin::set_cols(int num_cols) {
    if(num_cols < 1) {
        throw std::runtime_error("Cannot set the number of columns to less than 1.");
    }
    cols = num_cols;
}

void MemoryWin::update(BFCPUInterpreter &bfi, int len) {
    reinit(bfi, len);
}

uint16_t MemoryWin::get_tape_data(int index) {
    if(index < 0 || index >= tape_length) {
        throw std::runtime_error("Tape data index is out of bounds.");
    }
    if(!tape_data) {
        throw std::runtime_error("Tape date not initialized."); // NOTE: should never happen
    }
    return tape_data[index];
}

void MemoryWin::move_offset(int dir) {
    switch(dir) {
        case LEFT:
            tape_offset--;
            cursor_index = std::min(cursor_index + 1, tape_length - 1);
            break;
        case RIGHT:
            tape_offset++;
            cursor_index = std::max(cursor_index - 1, 0);
            break;
        case UP:
            tape_offset -= cols;
            if(cursor_index + cols < tape_length) {
                cursor_index += cols;
            }
            break;
        case DOWN:
            tape_offset += cols;
            if(cursor_index - cols >= 0) {
                cursor_index -= cols;
            }
            break;
    }
}

void MemoryWin::move_visual_cursor(int dir) {
    if(!visual_mode) {
        return;
    }
    switch(dir) {
        case LEFT:
            if(cursor_index == 0) {
                move_offset(UP);
                cursor_index = std::min(tape_length - 1, cols - 1);
            } else {
                cursor_index--;
            }
            break;
        case RIGHT:
            if(cursor_index >= tape_length - 1) {
                move_offset(DOWN);
                cursor_index = tape_length - 1 - ((tape_length - 1) % cols);
            } else {
                cursor_index++;
            }
            break;
        case UP:
            if(cursor_index < cols) {
                const int prev_ind = cursor_index;
                move_offset(UP);
                cursor_index = prev_ind;
            } else {
                cursor_index -= cols;
            }
            break;
        case DOWN:
            if(cursor_index >= tape_length - 1 - ((tape_length - 1) % cols)) {
                const int prev_ind = cursor_index;
                move_offset(DOWN);
                cursor_index = prev_ind;
            } else {
                cursor_index += cols;
            }
            break;
    }
}

bool MemoryWin::pointed_to(int index) {
    if(index < 0 || index >= tape_length) {
        throw std::runtime_error("Point index is out of bounds.");
    }
    uint16_t abs_tape_offset = static_cast<uint16_t>(index) + tape_offset;
    return page_offset == data_page_ptr && abs_tape_offset == data_tape_ptr;
}

void MemoryWin::set_offset(uint16_t tape_off, uint16_t page_off) {
    tape_offset = tape_off;
    page_offset = page_off;
}
