#pragma once

#include <cinttypes>

class Peripheral {
    public:
        virtual ~Peripheral() = default;

        virtual void write(uint16_t value) = 0;
        virtual uint16_t read() = 0;
};
