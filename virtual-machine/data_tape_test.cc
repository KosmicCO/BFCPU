#include "data_tape.hh"
#include <iostream> 

int main() {
    DataTape dt(17);
    uint16_t pages[] = {1, 6, 1, 10, 2};
    uint16_t indices[] = {19048, 65535, 0, 182};
    try {
        for(auto i = 0; i < 5; i++) {
            dt.set_page(pages[i]);
            std::cout << "For page: " << pages[i] << std::endl;
            std::cout << "  Read 0s test:" << std::endl;
            for(auto j = 0; j < 4; j++) {
                std::cout << "    " << dt.read(indices[j]) << std::endl;
            }
            std::cout << "  Write/Read test:" << std::endl;
            for(auto j = 0; j < 4; j++) {
                dt.write(indices[j], j ? 65535 : 23);
                std::cout << "    " << dt.read(indices[j]) << std::endl;
                dt.write(indices[j], 42);
            }
        }
    } catch(...) {
        return 1;
    }

    try {
        dt.set_page(18);
    } catch(...) {
        std::cout << "caught" << std::endl;
        return 2;
    }

    return 0;
}
