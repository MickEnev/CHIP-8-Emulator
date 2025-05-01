#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>
#include <array>

class Memory {
    public: 
    // Size of memory is 4kB or 4096 bytes or 8 * 4096 bits (not doing the math rn)

    static const unsigned int SIZE = 4096;
    Memory();
    // What should memory do
    // Read data
    uint8_t read(uint16_t address) const; // Address is 2 bytes but points to location in memory that is 1 byte
    // Write to that data
    void write(uint16_t address, uint8_t value); // So we take 2 byte address go to it and insert 1 byte value
    void LoadROM(const uint8_t* data, size_t size);
    void loadFontSet();
    
    private:
        std::array<uint8_t, SIZE> mem;

};

#endif