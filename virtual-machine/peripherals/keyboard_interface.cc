#include "keyboard_interface.hh"

void KeyboardInterface::write(uint16_t value) {
    switch(value) {
        case PCM_HARDWARE:
            cm_mode = CM_HARD;
            hardware_data->serialize(hardware_out);
            break;
        case PCM_KI_GET_KEYS:
            cm_mode = CM_READ;
            break;
    }
}

uint16_t KeyboardInterface::read() {
    std::scoped_lock lock(queue_mutex);
    uint16_t ret;
    switch(cm_mode) {
        case CM_HARD: 
            if(hardware_out.empty()) {
                return 0;
            }
            ret = hardware_out.front();
            hardware_out.pop();
            return ret;
        case CM_READ:
            if(keys.empty()) {
                return 0;
            }
            ret = keys.front();
            keys.pop();
            return ret;
    }
    return 0;
}

void KeyboardInterface::key_input(uint16_t key) {
    std::scoped_lock lock(queue_mutex);
    keys.push(key);
}
