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

void DataTape::touch_page(uint16_t t_page) {
    if(t_page >= num_pages){
        throw std::runtime_error("Page to touch is out of bounds.");
    }
    if(!memory[t_page]) {
        memory[t_page] = new uint16_t[SIZE];
        std::fill(memory[t_page], memory[t_page] + SIZE, 0);
    }
}

void DataTape::set_page(uint16_t new_page) {
    touch_page(new_page);
    page = new_page;
}

uint16_t DataTape::get_page() {
    return page;
}

int DataTape::get_num_pages() {
    return num_pages;
}

void DataTape::get_data(uint16_t start, uint16_t end, uint16_t * arr) {
    uint16_t mem_index = start;
    int arr_index = 0;
    while(mem_index != end) {
        arr[arr_index] = memory[page][mem_index];
        arr_index++;
        mem_index++;
    }

}

void DataTape::get_data_from_page(uint16_t start, uint16_t end, uint16_t * arr, uint16_t from_page) {
   touch_page(from_page);
   uint16_t mem_index = start;
   int arr_index = 0;
   while(mem_index != end) {
        arr[arr_index] = memory[from_page][mem_index];
        arr_index++;
        mem_index++;
   }

}

void DataTape::write_to_page(uint16_t address, uint16_t to_page, uint16_t value) {
    touch_page(to_page);
    memory[to_page][address] = value;
}
