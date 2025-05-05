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

void Chip8::setKeyState(uint8_t key, bool isPressed) {
    _keypad[key] = isPressed ? 1 : 0;
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
            if (_V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
                _PC += 2;
            }
            break;
        case 0x4000:
            if (_V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
                _PC += 2;
            }
            break;
        case 0x5000: { 
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t y = (opcode & 0x00F0) >> 4;
            if (_V[x] == _V[y]) {
                _PC += 2;
            }
            break;
        }
        case 0x6000:
            _V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF; // 0000 0000 0000 1011 
            break;
        case 0x7000:
            _V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            break;
        case 0x8000: {
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t y = (opcode & 0x00F0) >> 4;
            switch (opcode & 0x000F) {
                case 0x0000:
                    _V[x] = _V[y];
                    break;
                case 0x0001:
                    _V[x] |= _V[y];
                    break;
                case 0x0002:
                    _V[x] &= _V[y];
                    break;
                case 0x0003:
                    _V[x] ^= _V[y];
                    break;
                case 0x0004: {
                    if (_V[x] + _V[y] > 255) {
                        _V[15] = 1;
                    } else {
                        _V[15] = 0;
                    }
                    uint8_t res = (_V[x] + _V[y]) & 0xFF;
                    _V[x] = res;
                    break;
                }
                case 0x0005: {
                    if (_V[x] > _V[y]) {
                        _V[15] = 1;
                    } else {
                        _V[15] = 0;
                    }
                    uint8_t res = (_V[x] - _V[y]) & 0xFF;
                    _V[x] = res;
                    break;
                }
                case 0x0006:
                    _V[15] = _V[x] & 0x1;
                    _V[x] >>= 1;
                    break;
                case 0x0007:
                    if (_V[y] >= _V[x]) {
                        _V[15] = 1;
                    } else {
                        _V[15] = 0;
                    }
                    _V[x] = _V[y] - _V[x];
                    break;
                case 0x000E:
                    _V[15] = (_V[x] & 0x80) >> 7;
                    _V[x] <<= 1;
                    break;
                break;
            }
            break;
        }
        case 0x9000: {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t y = (opcode & 0x00F0) >> 4;
            if (_V[x] != _V[y]) {
                _PC += 2;
            }
            break;
        }
        case 0xA000:
            _I = opcode & 0x0FFF;
            break;
        case 0xB000:
            _PC = _V[0] + (opcode & 0x0FFF);
            break;
        case 0xC000: {
            uint8_t x = (opcode & 0x0F00) >> 8;
            _V[x] = (rand() % 256) & (opcode & 0x00FF);
            break;
        }
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
                //std::cout << "I=" << std::hex << +_I << " x=" << +x << " y=" << +y << " byte=" << +spriteByte << std::endl;
            }
            break;
        }
        case 0xE000:
            switch (opcode & 0x00FF) {
                case 0x9E:
                    if (_keypad[_V[(opcode & 0x0F00) >> 8]] != 0) {
                        _PC += 2;
                    }
                    break;
                case 0xA1:
                    if (_keypad[_V[(opcode & 0x0F00) >> 8]] == 0) {
                        _PC += 2;
                    }
                    break;
                default:
                    std::cerr << "Unknown 0xE000 opcode: " << std::hex << opcode << std::endl;
                    break;
            }
            break;
        
        case 0xF000: {
            uint8_t x = (opcode & 0x0F00) >> 8;
            switch (opcode & 0x00FF) {
                case 0x07:
                    _V[x] = _delayTimer;
                    break;
                case 0x0A: {
                    bool keyPressed = false;
                    for (int i = 0; i < 16; i++) {
                        if (_keypad[i] != 0) {
                            _V[x] = i;
                            keyPressed = true;
                            break;
                        }
                    }
                    if (!keyPressed) {
                        _PC -= 2;
                    }
                    break;
                }
                case 0x15:
                    _delayTimer = _V[x];
                    break;
                case 0x18:
                    _soundTimer = _V[x];
                    break;  
                case 0x1E:
                    _I += _V[x];
                    break;
                case 0x29:
                    _I = _V[x] * 5;
                    break;
                case 0x33:
                    _memory.write(_I,     _V[x] / 100);
                    _memory.write(_I + 1, (_V[x] / 10) % 10);
                    _memory.write(_I + 2, _V[x] % 10);
                    break;
                case 0x55:
                    for (int i = 0; i <= x; ++i) {
                        _memory.write(_I + i, _V[i]);
                    }
                    break;
                case 0x65:
                    for (int i = 0; i <= x; ++i) {
                        _V[i] = _memory.read(_I + i);
                    }
                    break;
                default:
                std::cerr << "Unknown 0xF000 opcode" << std::hex << opcode << std::endl;
                break;
            }
            break;
        }
        
        default:
            std::cerr << "Unknown opcode: " << std::hex << opcode << std::endl;
    }
}