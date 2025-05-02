#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL3/SDL.h>
#include <cstdint>

class Display {
    public:
    static const int VIDEO_WIDTH = 64;
    static const int VIDEO_HEIGHT = 32;
    static const int SCALE = 10;

    Display();
    ~Display();

    void clear();
    void draw(const uint8_t* videoBuffer);

    private:
    SDL_Window* _window;
    SDL_Renderer* _renderer;

};

#endif