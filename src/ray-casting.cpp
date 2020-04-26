/*============================================================================
 *  raycasting - Trung Truong
 *
 *  File name: raycasting.cpp
 *
 *  Description: Implementation of ray-casting engine. This program renders 
 *  "3D" terrain based on provided map. Think retro Wolfenstein 3D.
 * 
 *  Usage: ./raycast
 * 
 *==========================================================================*/
#include <iostream>
#include <vector>
#include <assert.h>
#include <SDL2/SDL.h>

/* Constants */
const int w_width = 600;                            // Window width
const int w_height = 480;                           // Window height
const int minimap_size = 5;                         // Mini-map tile size

/* 15 x 15 Hard-coded maze: each '#' is a wall */
const std::vector<std::vector<char> > maze = {
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
};

/* Render mini-map based on the player position */
void render_minimap(SDL_Renderer *renderer) {
    assert(maze.size() > 0);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); 
    SDL_RenderClear(renderer);

    for (size_t row = 0; row < maze.size(); row++) {
        for (size_t col = 0; col < maze[0].size(); col++) {
            SDL_Rect tile;
            if (maze[row][col] == '#') {
                SDL_SetRenderDrawColor(renderer, 150, 150, 150, 200);
                tile.x = row * minimap_size;
                tile.y = col * minimap_size;
                tile.w = minimap_size;
                tile.h = minimap_size; 
            }
            else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                tile.x = row * minimap_size;
                tile.y = col * minimap_size;
                tile.w = minimap_size;
                tile.h = minimap_size;  
            }
            SDL_RenderDrawRect(renderer, &tile);
        }
    }
    SDL_RenderPresent(renderer);
}

/* Main loop */
int main() {
    SDL_Renderer *renderer;
    SDL_Window *window;

    // SDL2 Initialization
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(w_width, w_height, 0, &window, &renderer);
    
    // Player position
    // float player_pos_x = 0;
    // float player_pos_y = 0;

    bool stop_simulation = false;
    
    while (!stop_simulation) {
        SDL_Event event;
        SDL_PollEvent(&event);

        switch (event.type) {
            case SDL_QUIT:
                stop_simulation = true;
                break;

            case SDL_KEYDOWN: {
                if (event.key.keysym.scancode == SDL_SCANCODE_UP) { 
                    render_minimap(renderer);
                }
            }

            default:
                break;
        }
    }
    
    // Destructor
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
