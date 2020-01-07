#include "hardware_info.hh"

HardwareInfo::HardwareInfo() {
    base_hardware_type = HARDWARE_NONE;
}

void HardwareInfo::serialize(std::queue<uint16_t> &queue) {
    std::queue<uint16_t> empty;
    std::swap(queue, empty);
    queue.push(base_hardware_type);
}
