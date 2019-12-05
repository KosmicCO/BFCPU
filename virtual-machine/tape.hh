#pragma once

#include <cinttypes>

class Tape {
    public:
        virtual ~Tape() = default;

        virtual uint16_t read(uint16_t address) = 0;
        virtual void write(uint16_t address, uint16_t value) = 0;
};
