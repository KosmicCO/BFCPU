#include "peripheral_tape.hh"
#include <iostream>
#include <vector>

class TestPeripheral : public Peripheral {
    public:
        TestPeripheral(uint16_t d) : id(d) {}
        ~TestPeripheral() = default;

        void write(uint16_t value) { std::cout << "wrote: " << value << std::endl; }
        uint16_t read() { return id; } 
    private:
        uint16_t id;
};

int main() {
    std::vector<Peripheral *> perphs;
    for(auto i = 0; i < 32; i++) {
        perphs.push_back(new TestPeripheral(i + 1));
    }
    PeripheralTape pt(perphs);
    uint16_t indices[] = {1, 9, R_NUM_DEVICES, 3, 1824, R_NUM_PAGES, 9, 2384};
    for(auto i = 0; i < 7; i++) {
        std::cout << "For addr: " << indices[i] << std::endl;
        std::cout << "  Read: " << pt.read(indices[i]) << std::endl;
        std::cout << "  Write 17: ";
        pt.write(indices[i], 17);
    }
    
    for(Peripheral * p : perphs) {
        delete p;
    }

    return 0;
}
