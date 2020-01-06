#pragma once

#include <cinttypes>

const uint16_t PCM_HARDWARE = 0;

class Peripheral {
    public:
        virtual ~Peripheral() = default;

        virtual void write(uint16_t value) = 0;
        virtual uint16_t read() = 0;
};
