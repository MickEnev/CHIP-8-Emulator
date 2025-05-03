#include "chip8.h"
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include <vector>

Chip8::Chip8() : _I(0), _PC(0x200), _delayTimer(0), _soundTimer(0)
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
        throw std::runtime_error("Failed to read ROM");
    }

    _memory.loadROM(reinterpret_cast<uint8_t*>(buffer.data()), size);

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
            _stack.push(_PC);
            _PC = opcode & 0x0FFF;
            break;
        case 0x3000:
            if (_V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
                _PC += 2;
            }
            break;
        case 0x6000:
            _V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF; // 0000 0000 0000 1011 
            break;
        case 0x7000:
            _V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            break;
        case 0xA000:
            _I = opcode & 0x0FFF;
            break;
        case 0XD000: {
            uint8_t x = _V[(opcode & 0x0F00) >> 8];
            uint8_t y = _V[(opcode & 0x00F0) >> 4];
            uint8_t height = opcode & 0x000F;
            _V[0xF] = 0;

            for (int row = 0; row < height; row++) {
                uint8_t spriteByte = _memory.read(_I + row);
                for (int col = 0; col < 8; col++) {
                    if ((spriteByte & (0x80 >> col)) != 0) {
                        int index = ((y + row) % 32) * 64 + ((x + col) % 64);
                        if (_video[index] == 1) {
                            _V[0xF] = 1;
                        }
                        _video[index] ^= 1;
                    }
                }
                std::cout << "I=" << std::hex << +_I << " x=" << +x << " y=" << +y << " byte=" << +spriteByte << std::endl;
            }
            
            break;
        }
        default:
            std::cerr << "Unknown opcode: " << std::hex << opcode << std::endl;
    }
}