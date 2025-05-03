#include "chip8.h"
#include <fstream>
#include <stdexcept>

Chip8::Chip8() : _I(0), _PC(0), _delayTimer(0), _soundTimer(0)
    {
        // Clear memory
        std::fill(std::begin(_V), std::end(_V), 0);
        std::fill(std::begin(_video), std::end(_video), 0);
        std::fill(std::begin(_keypad), std::end(_keypad), 0);

        _memory.loadFontSet();
    }

void Chip8::loadROM(const char* filename) {
    // TODO: Figure out how to load a rom... LOL
}

void Chip8::cycle() {
    // Fetch
    uint16_t opcode = (_memory.read(_PC) << 8 | _memory.read(_PC + 1));
    _PC += 2;

    // Decode and execute
    executeOpcode(opcode);
}

void Chip8::updateTimers() {
    if (_delayTimer > 0) --_delayTimer;
    if (_soundTimer > 0) --_soundTimer;
}