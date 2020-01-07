#pragma once

#include <cinttypes>
#include <queue>
#include <algorithm>

const uint16_t HARDWARE_NONE = 0;
const uint16_t HARDWARE_KEYBOARD = 1;
const uint16_t HARDWARE_TERMINAL = 2;
const uint16_t HARDWARE_NETWORK = 3;

class HardwareInfo {
    public:
        HardwareInfo();
        ~HardwareInfo() = default;

        void serialize(std::queue<uint16_t> &queue);

        void set_base_hardware_type(uint16_t id) { base_hardware_type = id; }
    private:
        uint16_t base_hardware_type;
};
