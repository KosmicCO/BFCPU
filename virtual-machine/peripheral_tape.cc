#include "peripheral_tape.hh"
#include "bfcpu_interpreter.hh"

uint16_t PeripheralTape::read(uint16_t address) {
    if(address < num_devices()) {
        return devices[address]->read();
    }
    switch(address) {
        case R_NUM_DEVICES: return static_cast<uint16_t>(num_devices());
        case R_NUM_PAGES:   if(native_perphs) { return native_perphs->get_num_pages(); }    break;
        case R_CLOCK:       if(native_perphs) { return native_perphs->get_time(); }         break;
        case R_PAGE_PTR:    if(native_perphs) { return native_perphs->get_page_ptr(); }     break;
        case R_DATA_PTR:    if(native_perphs) { return native_perphs->get_data_ptr(); }     break;
    }
    return 0;
}

void PeripheralTape::write(uint16_t address, uint16_t value) {
    if(address < num_devices()) {
        devices.at(address)->write(value);
    } else {
        switch(address) {
            case W_SET_CLOCK:   if(native_perphs) { native_perphs->set_time(value); }    break;
        }
    }
}

int PeripheralTape::num_devices() const {
    return devices.size();
}
