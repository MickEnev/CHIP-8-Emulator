#ifndef CHIP8_H
#define CHIP8_H

#include "memory.h"
#include "stack.h"
#include <cstdint>

class Chip8 {
    public:
    Chip8();
    void loadROM(const char* filename);
    void cycle();           // Fetch, decode, execute
    void updateTimers();    // Called at 60Hz
    uint8_t* getVideoBuffer();
    void setVideoBuffer(uint8_t buffer[]); // For testing
    void testExecuteOpcode(uint16_t opcode); // For Testing

    private:
    Memory _memory;
    Stack _stack;

    // Registers
    uint8_t _V[16];  // V0 to VF

    // Index register and program counter
    uint16_t _I;
    uint16_t _PC;

    // Timers
    uint8_t _delayTimer;
    uint8_t _soundTimer;

    // Display buffer
    uint8_t _video[64 * 32];

    // input
    uint8_t _keypad[16];

    // Fetch, decode, execute helpers
    void executeOpcode(uint16_t opcode);
    
};

#endif