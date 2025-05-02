#include "display.h"

Display::Display() {
    SDL_Init(SDL_INIT_VIDEO);

    _window = SDL_CreateWindow(
        "CHIP-8 Emulator", 
        VIDEO_WIDTH * SCALE,
        VIDEO_HEIGHT * SCALE,
        SDL_WINDOW_OPENGL
    );

    _renderer = SDL_CreateRenderer(_window, nullptr);
}

Display::~Display() {
    SDL_DestroyRenderer(_renderer);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void Display::clear() {
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);
}

void Display::draw(const uint8_t* videoBuffer) {
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
    
    for (int y = 0; y < VIDEO_HEIGHT; y++) {
        for (int x = 0; x < VIDEO_WIDTH; x++) {
            if (videoBuffer[y * VIDEO_WIDTH + x]) {
                SDL_FRect pixelRect = {
                    x * SCALE,
                    y * SCALE,
                    SCALE,
                    SCALE
                };
                SDL_RenderFillRect(_renderer, &pixelRect);
            }
        }
    }
    SDL_RenderPresent(_renderer);
}


