#pragma once

#include "tape.hh"
#include "peripheral.hh"
#include <vector>

class BFCPUInterpreter;

const uint16_t NUM_END_READS = 5; 

const uint16_t R_NUM_DEVICES = -5;
const uint16_t R_NUM_PAGES = -4;
const uint16_t R_CLOCK = -3;
const uint16_t R_PAGE_PTR = -2;
const uint16_t R_DATA_PTR = -1;

const uint16_t NUM_END_WRITES = 1;

const uint16_t W_SET_CLOCK = -1;

class PeripheralTape : public Tape {
    public:
        PeripheralTape(const std::vector<Peripheral *> &periphs) : devices(periphs), native_perphs(nullptr) {};
        PeripheralTape(const std::vector<Peripheral *> &periphs, BFCPUInterpreter * na_perphs) : devices(periphs), native_perphs(na_perphs) {};
        ~PeripheralTape() = default;

        uint16_t read(uint16_t address);
        void write(uint16_t address, uint16_t value);
        virtual int num_devices() const;
    private:
        std::vector<Peripheral *> devices;
        BFCPUInterpreter * native_perphs;
};
