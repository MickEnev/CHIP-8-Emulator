#include "chip8.h"
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <vector>

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
    std::ifstream rom(filename, std::ios::binary | std::ios::ate);
    if (!rom) throw std::runtime_error("Failed to open ROM");

    std::streamsize size = rom.tellg();
    rom.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (!rom.read(buffer.data(), size)) {
        // Left off here before meeting with swarup
    }

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

uint8_t* Chip8::getVideoBuffer() {
    return _video;
}

// For Testing ------------------
void Chip8::setVideoBuffer(uint8_t buffer[]) {
    for (int i = 0; i < 64 * 32; i++) {
        _video[i] = buffer[i];
    }
}

void Chip8::testExecuteOpcode(uint16_t opcode) {
    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x00FF) {
                case 0x00E0: // Display clear
                    memset(_video, 0, sizeof(_video));
                    _V[15] = 1; // Set VF to true when pixel gets turned off MIGHT BE WRONG
                    break;
                case 0x00EE: // Returns from subroutine
                    _PC = _stack.top();
                    _stack.pop();
                    break;
                default:
                    std::cerr << "Unknown opcode: " << std::hex << opcode << std::endl;
                    
            }
            break;
            
        default:
            std::cerr << "Unknown opcode: " << std::hex << opcode << std::endl;
    }
}
// For Testing --------------------

void Chip8::executeOpcode(uint16_t opcode) {
    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x00FF) {
                case 0x00E0: // Display clear
                    memset(_video, 0, sizeof(_video));
                    _V[15] = 1; // Set VF to true when pixel gets turned off MIGHT BE WRONG
                    break;
                case 0x00EE: // Returns from subroutine
                    _PC = _stack.top();
                    _stack.pop();
                    break;
                default:
                    std::cerr << "Unknown opcode: " << std::hex << opcode << std::endl;
                    
            }
            break;
        
        case 0x1000:
            _PC = opcode & 0x0FFF;
            break;
        case 0x2000:
            _stack.push(opcode & 0x0FFF);
            _PC = _stack.top();
            break;
        case 0x3000: // TODO: Come back to this and check that its right
            if (_V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
                _stack.pop();
                _PC = _stack.top();
            }
        // TODO 3 4 and 5
        case 0x6000:
            _V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF; // 0000 0000 0000 1011 
            break;
        case 0x7000:
            _V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            break;

        default:
            std::cerr << "Unknown opcode: " << std::hex << opcode << std::endl;
    }
}