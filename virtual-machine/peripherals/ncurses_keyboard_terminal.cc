#include "ncurses_keyboard_terminal.hh"

const int MAX_LINES = 500;

const int READING_NONE = 0;
const int READING_HARD = 1;
const int READING_TERM = 2;

NCursesKeyboardTerminal * NCursesKeyboardTerminal::instance = nullptr;

NCursesKeyboardTerminal * NCursesKeyboardTerminal::get_instance() {
    if(instance == nullptr) {
        instance = new NCursesKeyboardTerminal();
    }
    return instance;
}

NCursesKeyboardTerminal::NCursesKeyboardTerminal() :
    terminal_hardware_data(),
    running(true),
    lines(),
    hardware_out(),
    term_out(),
    last_comm(-1),
    reading(READING_NONE)
{
    lines.push_front(new std::list<wchar_t>());

    terminal_hardware_data.set_base_hardware_type(HARDWARE_TERMINAL);

    keyboard_hardware_info = std::shared_ptr<HardwareInfo>(new HardwareInfo());
    keyboard_hardware_info->set_base_hardware_type(HARDWARE_KEYBOARD);
    keyboard = std::shared_ptr<KeyboardInterface>(new KeyboardInterface(keyboard_hardware_info));

    setlocale(LC_ALL, "");

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    timeout(-1);
    curs_set(0);
    ESCDELAY = 0;

    run_keyboard = new std::thread([this]() {
        while(running) {
            int c = getch();
            keyboard->key_input(c);
        }
    });
    
}

NCursesKeyboardTerminal::~NCursesKeyboardTerminal() {
    running = false;
    run_keyboard->join();
    delete run_keyboard;
    for(auto ln : lines) {
        delete ln;
    }
    endwin();
}

void NCursesKeyboardTerminal::ready_term_out() {
    std::queue<uint16_t> empty;
    std::swap(term_out, empty);
    term_out.push(static_cast<uint16_t>(COLS));
    term_out.push(static_cast<uint16_t>(LINES));
    // TODO: also push flags about what the terminal supports
}

void NCursesKeyboardTerminal::render() {
    auto lns_index = lines.begin();
    auto lns_end = lines.end();
    clear();
    for(int i = std::min(LINES - 1, static_cast<int>(lines.size())); i >=0 && lns_index != lns_end; i--) {
        move(i, 0);
        auto chs_index = (*lns_index)->begin();
        auto chs_end = (*lns_index)->end();
        for(int j = 0; j < COLS && chs_index != chs_end; j++) {
            cchar_t pc;
            std::wstring single;
            single.push_back(*chs_index);
            setcchar(&pc, single.c_str(), 0, 0, nullptr);
            add_wch(&pc);
            chs_index++;
        }
        lns_index++;
    }
    refresh();
}

void NCursesKeyboardTerminal::print_char(uint16_t c) {
    switch(c) {
        case '\0':
            last_comm = -1;
            break;
        case '\b':
        case 0x7F:
        case 0x94:
            if(!lines.front()->empty()) {
                lines.front()->pop_back();
            }
            break;
        case '\n':
            lines.push_front(new std::list<wchar_t>());
            if(lines.size() > MAX_LINES) {
                std::list<wchar_t> * back = lines.back();
                lines.pop_back();
                delete back;
            }
            break;
        case '\f':
            for(auto ln : lines) {
                delete ln;
            }
            lines.clear();
            lines.push_front(new std::list<wchar_t>());
            break;
        case '\r':
            lines.front()->clear();
            break;
        default:
            if(!(c < 0x20 || (c >= 0x7F && c < 0xA0))) {
                lines.front()->push_back(static_cast<wchar_t>(c));
            }
    }
}

void NCursesKeyboardTerminal::write(uint16_t value) {
    switch(last_comm) {
        case -1:
            switch(value) {
                case PCM_HARDWARE:
                    terminal_hardware_data.serialize(hardware_out);
                    reading = READING_HARD;
                    break;
                case PCM_TI_SEND_CHARS:
                    last_comm = PCM_TI_SEND_CHARS;
                    reading = READING_NONE;
                    break;
                case PCM_TI_TERM_INFO:
                    reading = READING_TERM;
                    ready_term_out();
                    break;
                case PCM_TI_SEND_COMM:
                    last_comm = PCM_TI_SEND_COMM;
                    reading = READING_NONE;
                    break;
            }
            break;
        case PCM_TI_SEND_CHARS:
            print_char(value);
            break;
        case PCM_TI_SEND_COMM:
            // TODO: take in commands
            last_comm = -1;
            break;
    }
}

uint16_t NCursesKeyboardTerminal::read() {
    uint16_t ret;
    switch(reading) {
        case READING_HARD:
            if(hardware_out.empty()) {
                return 0;
            }
            ret = hardware_out.front();
            hardware_out.pop();
            return ret;
        case READING_TERM:
            if(term_out.empty()) {
                return 0;
            }
            ret = term_out.front();
            term_out.pop();
            return ret;
    }
    return 0;
}





