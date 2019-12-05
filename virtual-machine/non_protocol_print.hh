#pragma once

#include "peripheral.hh"

class NonProtocolPrint : public Peripheral {
    public:
        NonProtocolPrint() {}
        ~NonProtocolPrint() = default;

        void write(uint16_t value);
        uint16_t read();
};
