#include "non_protocol_print.hh"
#include <iostream>

void NonProtocolPrint::write(uint16_t value) {
    std::cout << static_cast<char>(0x00FF & value);
}

uint16_t NonProtocolPrint::read() {
    return 0;
}
