/*============================================================================
 *  conway - Trung Truong
 *
 *  File name: conway.c
 *
 *  Description: Implementation of Conway's Game of Life. RIP
 *  Usage: ./conway
 * 
 *==========================================================================*/
#include <assert.h>
#include <SDL2/SDL.h>

const int w_width = 600;
const int w_height = 480;
const int cell_size = 5;

int main() {
    SDL_Renderer *renderer;
    SDL_Window *window;

    // SDL2 initialization
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(w_width, w_height, 0, &window, &renderer);
    
    // Intialize random number generator
    time_t t;
    srand((unsigned) time(&t));

    // Initialize 2D array map for game of life
    int map_width  = w_width  / cell_size;
    int map_height = w_height / cell_size; 

    char *map_state  = calloc(map_width * map_height, sizeof(char));
    assert(map_state);
    char *map_output = calloc(map_width * map_height, sizeof(char));
    assert(map_output);

    // Init random map
    for (int r = 0; r < map_height; r++) {
        for (int c = 0; c < map_width; c++) {
            if (rand() % 8 == 1) 
                map_state[r * map_width + c] = 1;
            else 
                map_state[r * map_width + c] = 0;
        }
    }

    // Game of life main loop
    while (1) {
        SDL_Event event;
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) break;

        for (int i = 0; i < map_width * map_height; i++)
            map_output[i] = map_state[i];

        for (int r = 1; r < map_height - 1; r++) {
            for (int c = 1; c < map_width - 1; c++) {
                // Get number of alive neighbors
                int alive_neighbors = 
                    map_output[(r-1) * map_width + (c-1)] +
                    map_output[(r-1) * map_width +  c   ] +
                    map_output[(r-1) * map_width + (c+1)] +
                    map_output[   r  * map_width + (c-1)] +
                    map_output[   r  * map_width + (c+1)] +
                    map_output[(r+1) * map_width + (c-1)] +
                    map_output[(r+1) * map_width +  c   ] +
                    map_output[(r+1) * map_width + (c+1)];

                // Update cell state based on number of alive neighbors
                if ((int) map_output[r * map_width + c] == 1) {
                    map_state[r * map_width + c] =
                        (alive_neighbors == 2 || alive_neighbors == 3);
                    SDL_SetRenderDrawColor(renderer, 150, 235, 55, 255);              
                }
                else {
                    map_state[r * map_width + c] = alive_neighbors == 3;
                    SDL_SetRenderDrawColor(renderer, 10, 24, 35, 20);   
                }

                // Display cell
                SDL_Rect tile;
                tile.x = c * cell_size;
                tile.y = r * cell_size;
                tile.w = cell_size;
                tile.h = cell_size;
                SDL_RenderFillRect(renderer, &tile);
                SDL_RenderDrawRect(renderer, &tile); 
            }
        }
        SDL_RenderPresent(renderer);
    }

    // Memory cleanup
    free(map_state);
    free(map_output);

    // SDL2 clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
