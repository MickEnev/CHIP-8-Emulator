#include <SDL3/SDL.h>
#include <iostream>
#include <array>
#include <cstdint>
#include <cstring>
#include "memory.h"
#include "display.h"
#include "chip8.h"

int main(int argc, char* argv[]) {
    std::cout << "SDL WORKS" << std::endl;
    Memory memTest = Memory();
    std::cout << memTest.read(0x050) << std::endl;

    Display display;
    bool done = false;

    Chip8 cpu;

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
    /*
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