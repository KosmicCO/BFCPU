#pragma once
#include "../peripheral.hh"
#include <queue>
#include <mutex>
#include "hardware_info.hh"
#include <memory>

const uint16_t PCM_KI_GET_KEYS = 1;

const int CM_NONE = 0;
const int CM_HARD = 1;
const int CM_READ = 2;

class KeyboardInterface : public Peripheral {
    public:
        KeyboardInterface(std::shared_ptr<HardwareInfo> hi) : hardware_data(hi), hardware_out(), keys(), cm_mode(CM_NONE), queue_mutex() {}
        ~KeyboardInterface() {}

        void write(uint16_t value);
        uint16_t read();

        void key_input(uint16_t key);
    private:
        std::shared_ptr<HardwareInfo> hardware_data;
        std::queue<uint16_t> hardware_out;
        std::queue<uint16_t> keys;
        int cm_mode;
        std::mutex queue_mutex;
};
