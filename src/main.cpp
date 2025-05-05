#include <SDL3/SDL.h>
#include <iostream>
#include <array>
#include <cstdint>
#include <cstring>
#include "memory.h"
#include "display.h"
#include "chip8.h"

int main(int argc, char* argv[]) {
    const int INSTRUCTIONS_PER_SECOND = 700;
    const int FRAME_RATE = 60;

    uint32_t lastCycleTime = SDL_GetTicks();
    uint32_t lastTimerUpdate = SDL_GetTicks();

    Display display;
    bool done = false;

    Chip8 cpu;
    cpu.loadROM("ROMS/Airplane.ch8");

    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                done = true;
            }

            if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
                bool isPressed = event.type == SDL_EVENT_KEY_DOWN;
                
                switch (event.key.key) {
                    case SDLK_1:  cpu.setKeyState(0x1, isPressed); break;
                    case SDLK_2:  cpu.setKeyState(0x2, isPressed); break;
                    case SDLK_3:  cpu.setKeyState(0x3, isPressed); break;
                    case SDLK_4:  cpu.setKeyState(0xC, isPressed); break;

                    case SDLK_Q:  cpu.setKeyState(0x4, isPressed); break;
                    case SDLK_W:  cpu.setKeyState(0x5, isPressed); break;
                    case SDLK_E:  cpu.setKeyState(0x6, isPressed); break;
                    case SDLK_R:  cpu.setKeyState(0xD, isPressed); break;

                    case SDLK_A:  cpu.setKeyState(0x7, isPressed); break;
                    case SDLK_S:  cpu.setKeyState(0x8, isPressed); break;
                    case SDLK_D:  cpu.setKeyState(0x9, isPressed); break;
                    case SDLK_F:  cpu.setKeyState(0xE, isPressed); break;

                    case SDLK_Z:  cpu.setKeyState(0xA, isPressed); break;
                    case SDLK_X:  cpu.setKeyState(0x0, isPressed); break;
                    case SDLK_C:  cpu.setKeyState(0xB, isPressed); break;
                    case SDLK_V:  cpu.setKeyState(0xF, isPressed); break;
                }
            }
        }
            uint32_t currentTime = SDL_GetTicks();
            if (currentTime - lastCycleTime > 1000 / INSTRUCTIONS_PER_SECOND) {
                cpu.cycle();
                lastCycleTime = currentTime;
            }
            
            // Update timers at 60Hz
            if (currentTime - lastTimerUpdate > 1000 / 60) {
                cpu.updateTimers();
                lastTimerUpdate = currentTime;
            }
            
            // Render at the target frame rate
            display.clear();
            display.draw(cpu.getVideoBuffer());
            
            // Add a small delay to prevent CPU hogging
            SDL_Delay(1);
    }

    /*
    // Test video buffer of rectangle
    uint8_t videoBuffer[64 * 32];
    std::memset(videoBuffer, 0, sizeof(videoBuffer));
    for (int y = 10; y < 20; y++) {
        for (int x = 20; x < 40; x++) {
            videoBuffer[y * 64 + x] = 1;
        }
    }
    
    cpu.setVideoBuffer(videoBuffer);

    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                done = true;
            }
        }
        display.clear();
        cpu.setVideoBuffer(videoBuffer);
        display.draw(cpu.getVideoBuffer());

        display.clear();
        cpu.testExecuteOpcode(0x00E0);
        display.draw(cpu.getVideoBuffer());
    }


    SDL_Window *window;
    bool done = false;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "An SDL3 window", 
        640,
        320,
        SDL_WINDOW_OPENGL
    );

    if (window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "COuld not create window; %s\n", SDL_GetError());
        return 1;
    }

    while (!done) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                done = true;
            }
        }

        // Game logic
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    */
    
    return 0;
}