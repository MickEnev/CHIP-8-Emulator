#include <SDL3/SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "SDL WORKS" << std::endl;

    SDL_Window *window;
    bool done = false;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "An SDL3 window", 
        640,
        480,
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

    return 0;
}