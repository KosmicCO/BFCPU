#include "bfcpu_interpreter.hh"

bool is_bfcpu_char(char c) {
    switch(c) {
        case '>':
        case '<':
        case '+':
        case '-':
        case '[':
        case ']':
        case '.':
        case ',':
        case '@':
        case '^':
        case '*':
        case '~':
        case '&':
        case '#':
        case '}':
        case '{': return true;
        default: return false;
    }
}

bool match_parens(char * prog, int len) {
    int unclosed = 0;
    for(auto i = 0; i < len; i++) {
        switch(prog[i]) {
            case '[': unclosed++; break;
            case ']': unclosed--; break;
        }
        if(unclosed < 0) {
            return false;
        }
    }
    return unclosed == 0;
}

int * create_jumps(char * prog, int len) {
    int * jumps = new int[len];
    std::stack<int> backs;
    for(auto i = 0; i < len; i++) {
        jumps[i] = 0;
        if(prog[i] == '[') {
            backs.push(i);
        } else if(prog[i] == ']') {
            int match = backs.top();
            backs.pop();
            jumps[i] = match;
            jumps[match] = i;
        }
    }
    return jumps;
}

void BFCPUInterpreter::construct_sequence(std::istream &rom, const std::vector<Peripheral *> &perphs, int num_pages, bool strip) {
    prog = new char[ROM_SIZE];
    std::fill(prog, prog + ROM_SIZE, 0);
    auto i = 0;
    while(rom) {
        if(i >= ROM_SIZE) {
            throw std::runtime_error("Program too large to fit on ROM.");
        }
        char c = rom.get();
        if(!strip || is_bfcpu_char(c)) {
            prog[i] = c;
            i++;
        }
    }
    prog_len = i;
    if(!match_parens(prog, prog_len)) {
        throw std::runtime_error("Program has unmatching parenthesis");
    }

    jumps = create_jumps(prog, prog_len);
    prog_ptr = 0;
    data_ptr = 0;
    buffer = 0;

    clock_time = 0;
    
    data_tape = new DataTape(num_pages);
    perp_tape = new PeripheralTape(perphs, this);
}

BFCPUInterpreter::BFCPUInterpreter(std::istream &rom, const std::vector<Peripheral *> &perphs, int num_pages, bool strip) {
    construct_sequence(rom, perphs, num_pages, strip);
}

BFCPUInterpreter::BFCPUInterpreter(const std::string &file, const std::vector<Peripheral *> &perphs, int num_pages, bool strip) {
    std::ifstream rom_prog;
    rom_prog.open(file);
    if(!rom_prog) {
        throw std::runtime_error("Could not read from file.");
    }

    construct_sequence(rom_prog, perphs, num_pages, strip);
    rom_prog.close();
}

BFCPUInterpreter::~BFCPUInterpreter() {
    delete[] prog;
    delete[] jumps;
    delete data_tape;
    delete perp_tape;
}



bool BFCPUInterpreter::next(bool comm) {
    // TODO: finish
    // TODO: also have each of the private helper functions apply a change to the program counter

    if(prog_ptr >= prog_len) {
        return false;
    }

    char c = prog[prog_ptr];

    switch(c) {
        case '>': move_right();     break;
        case '<': move_left();      break;
        case '+': increment();      break;
        case '-': decrement();      break;
        case '.': write();          break;
        case ',': read();           break;
        case '[': loop_forward();   break;
        case ']': loop_back();      break;
        case '@': jump_data();      break;
        case '^': copy();           break;
        case '*': paste();          break;
        case '~': left_shift();     break;
        case '&': nand();           break;
        case '#': jump_page();      break;
        case '}': page_right();     break;
        case '{': page_left();      break;
        default:  pinc();           return true;
    }

    clock_time++;

    if(comm) {
        c = prog[prog_ptr];

        while(!is_bfcpu_char(c)) {
            if(prog_ptr >= prog_len) {
                return false;
            }
            pinc();
            c = prog[prog_ptr];
        }
    }

    return true;
}

uint16_t BFCPUInterpreter::get_time() {
    return clock_time;
}

void BFCPUInterpreter::set_time(uint16_t ntime) {
    clock_time = ntime;
}

uint16_t BFCPUInterpreter::get_num_pages() {
    return data_tape->get_num_pages();
}

uint16_t BFCPUInterpreter::get_page_ptr() {
    return data_tape->get_page();
}

uint16_t BFCPUInterpreter::get_data_ptr() {
    return data_ptr;
}

uint16_t BFCPUInterpreter::get_prog_ptr() {
    return prog_ptr;
}

void BFCPUInterpreter::get_tape_data(uint16_t start, uint16_t end, uint16_t * arr) {
    data_tape->get_data(start, end, arr);
}

void BFCPUInterpreter::get_tape_data_from_page(uint16_t start, uint16_t end, uint16_t * arr, uint16_t from_page) {
    data_tape->get_data_from_page(start, end, arr, from_page);
}

uint16_t BFCPUInterpreter::at_ptr() {
    return data_tape->read(data_ptr);
}

void BFCPUInterpreter::pinc() {
    prog_ptr++;
}

void BFCPUInterpreter::move_left() {
    data_ptr--;

    pinc();
}

void BFCPUInterpreter::move_right() {
    data_ptr++;

    pinc();
}

void BFCPUInterpreter::increment() {
    data_tape->write(data_ptr, at_ptr() + 1);
    pinc();
}

void BFCPUInterpreter::decrement() {
    data_tape->write(data_ptr, at_ptr() - 1);

    pinc();
}

void BFCPUInterpreter::write() {
    perp_tape->write(buffer, at_ptr());

    pinc();
}

void BFCPUInterpreter::read() {
    data_tape->write(data_ptr, perp_tape->read(buffer));

    pinc();
}

void BFCPUInterpreter::loop_back() {
    if(!at_ptr()) {
        pinc();
    } else {
        prog_ptr = jumps[prog_ptr];
    }
}

void BFCPUInterpreter::loop_forward() {
    if(at_ptr()) {
        pinc();
    } else {
        prog_ptr = jumps[prog_ptr] + 1;
    }
}

void BFCPUInterpreter::jump_data() {
    data_ptr = at_ptr();

    pinc();
}

void BFCPUInterpreter::copy() {
    buffer = at_ptr();

    pinc();
}

void BFCPUInterpreter::paste() {
    data_tape->write(data_ptr, buffer);

    pinc();
}

void BFCPUInterpreter::left_shift() {
    uint16_t val = at_ptr();
    bool rot_bit = val & (1 << 15);
    val = val << 1;
    if(rot_bit) {
        val |= 1;
    }
    data_tape->write(data_ptr, val);
    
    pinc();
}

void BFCPUInterpreter::nand() {
    data_tape->write(data_ptr, ~(buffer & at_ptr()));

    pinc();
}

void BFCPUInterpreter::jump_page() { // NOTE: does nothing on invalid page
    uint16_t page = at_ptr();
    if(page < get_num_pages()) {
        data_tape->set_page(page);
    }

    pinc();
}

void BFCPUInterpreter::page_left() {
    uint16_t page = get_page_ptr();
    if(page == 0) {
        page = get_num_pages() - 1;
    } else {
        page--;
    }
    data_tape->set_page(page);

    pinc();
}

void BFCPUInterpreter::page_right() {
    uint16_t page = get_page_ptr();
    if(page == get_num_pages() - 1) {
        page = 0;
    } else {
        page++;
    }
    data_tape->set_page(page);

    pinc();
}
