#include "data_tape.hh"

DataTape::DataTape(uint16_t pages) {
    num_pages = pages;
    page = 0;
    memory = new uint16_t *[num_pages];
    std::fill(memory, memory + num_pages, nullptr);
    set_page(0); 
}

DataTape::~DataTape() {
    for(auto i = 0; i < num_pages; i++) {
        if(memory[i]){
            delete[] memory[i];
        }
    }
    delete[] memory;
}

uint16_t DataTape::read(uint16_t address) {
    return memory[page][address];
}

void DataTape::write(uint16_t address, uint16_t value) {
    memory[page][address] = value;
}

void DataTape::set_page(uint16_t new_page) {
    if(new_page >= num_pages) {
        throw 1;
    }
    page = new_page;
    if(!memory[page]) {
        memory[page] = new uint16_t[SIZE];
        std::fill(memory[page], memory[page] + SIZE, 0);
    }
}

uint16_t DataTape::get_page() {
    return page;
}

uint16_t DataTape::get_num_pages() {
    return num_pages;
}
