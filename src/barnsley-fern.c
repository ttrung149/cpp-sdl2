#include <stdlib.h>
#include <SDL2/SDL.h>

int main(void) {
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;

    // Intialize random number generator
    time_t t;
    srand((unsigned) time(&t));

    // SDL2 Initialization
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(600, 480, 0, &window, &renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    
    // Draw fern
    SDL_RenderClear(renderer);
    float x = 600, y = 480;
    int iterations = 50000;
    SDL_SetRenderDrawColor(renderer, 100, 150, 50, 255);

    for (int i = 0; i < iterations; i++) {
        float prev_x = x;
        float prev_y = y;
        int r = rand() % 100;

        if (r <= 1) {
            x = 0;
            y = 0.16 * prev_y;
        }
        else if (r < 86) {
            x = 0.85 * prev_x + 0.04 * prev_y;
            y = -0.04 * prev_x + 0.85 * prev_y + 1.6;
        }
        else if (r < 93) {
            x = 0.20 * prev_x - 0.26 * prev_y;
            y = 0.23 * prev_x + 0.22 * prev_y + 1.6;
        }
        else {
            x = -0.15 * prev_x + 0.28 * prev_y;
            y = 0.26 * prev_x + 0.24 * prev_y + 0.44;
        }
        SDL_RenderDrawPoint(renderer, 300 + 85 * x, 480 - 57 * y);
    }

    // Render screen 
    SDL_RenderPresent(renderer);
    
    // Destructor
    while (1) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
