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
#define DEBUG
#include <vector>
#include <assert.h>
#include <SDL2/SDL.h>
#include <math.h>

#ifdef DEBUG
#include <iostream>
#endif

/* Constants */
const int w_width = 600;                            // Window width
const int w_height = 480;                           // Window height
const int FPS = 60;
const int FPS_delay = 1000 / FPS;

/* 15 x 15 Hard-coded maze: each '#' is a wall */
const int minimap_tile_size = 5;                    // Mini-map tile size
const int maze_size = 15;
const std::vector<std::vector<char> > maze = {
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
    {'#',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ','#',' ',' ','#',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ','#','#','#','#',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ','#','#','#',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ','#'},
    {'#',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
};

/* Agent point of view attributes */
const int agent_size = 5;
const float pov = 3.14159 / 3;          // POV angle - pi/3
const float max_depth = 15;             // Max distance that agent can see

/* Render mini-map based on the agent position */
void render_minimap(SDL_Renderer *renderer, float x, float y, float a) {
    assert(maze.size() > 0);

    // Render wall and obstacles
    for (size_t row = 0; row < maze.size(); row++) {
        for (size_t col = 0; col < maze[0].size(); col++) {
            SDL_Rect tile;
            if (maze[col][row] == '#') {
                SDL_SetRenderDrawColor(renderer, 50, 50, 150, 200);
                tile.x = row * minimap_tile_size;
                tile.y = col * minimap_tile_size;
                tile.w = minimap_tile_size;
                tile.h = minimap_tile_size; 
            }
            else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                tile.x = row * minimap_tile_size;
                tile.y = col * minimap_tile_size;
                tile.w = minimap_tile_size;
                tile.h = minimap_tile_size;  
            }
            
            SDL_RenderFillRect(renderer, &tile);
            SDL_RenderDrawRect(renderer, &tile);
        }
    }

    // Render agent position
    SDL_SetRenderDrawColor(renderer, 150, 20, 20, 200);
    SDL_Rect agent;
    agent.x = x * agent_size;
    agent.y = y * agent_size;
    agent.w = agent_size;
    agent.h = agent_size;
    SDL_RenderFillRect(renderer, &agent);
    SDL_RenderDrawRect(renderer, &agent);

    // Render agent point of view
    SDL_SetRenderDrawColor(renderer, 250, 218, 94, 255);
    SDL_Rect agent_pov;
    agent_pov.x = x * agent_size + 5 * cosf(a);
    agent_pov.y = y * agent_size + 5 * sinf(a);
    
    agent_pov.w = 3;
    agent_pov.h = 3;
    SDL_RenderFillRect(renderer, &agent_pov);
    SDL_RenderDrawRect(renderer, &agent_pov);
}

/* Render terrain based on the agent position and view angle */
void render_terrain(SDL_Renderer *renderer, float x, float y, float a) {
    // Generate floor
    for (int r = w_height / 2; r < w_height; r += 3) {
        SDL_Rect floor;
        int color = r * 510 / w_height;
        SDL_SetRenderDrawColor(renderer, color, color, color, 150); 
        floor.x = 0;
        floor.y = r;
        floor.h = 3;
        floor.w = w_width;
        SDL_RenderFillRect(renderer, &floor);
        SDL_RenderDrawRect(renderer, &floor);
    }

    // Generate w_width number of rays for ray casting
    for (int i = 0; i < w_width; i++) {
        // Limit ray angle from -POV/2 to +POV/2
        float ray_angle = a - pov / 2.0 + ((float)i / (float)w_width) * pov;
        
        float distance_to_wall = 0;
        bool hit_wall = false;

        // Find distance from agent to wall in the direction of each ray
        while (!hit_wall && distance_to_wall < max_depth) {
            distance_to_wall += 0.05;

            // Assume generated ray vector has the form [(x, y), (rx, ry)],
            // where x, y is the current coordinates of the agent.

            // Calculate rx, ry based on distance to wall
            int rx = (int)(x + cosf(ray_angle) * distance_to_wall);
            int ry = (int)(y + sinf(ray_angle) * distance_to_wall);

            // If rx or ry is out of bounds, consider wall has been hit
            if (rx < 0 || rx > maze_size || ry < 0 || ry > maze_size) {
                hit_wall = true;
            }
            else {
                if (maze[ry][rx] == '#') {
                    hit_wall = true;
                }
            }
        }

        // Render wall tile - The further the wall, the darker its color
        SDL_Rect wall;
        float w_color = (distance_to_wall < 1) ? 255 : 255 / distance_to_wall; 
        SDL_SetRenderDrawColor(renderer, w_color, w_color, w_color, 255); 
        wall.x = i;
        wall.y = (w_height - w_height / distance_to_wall) / 2.0;
        wall.w = 1;
        wall.h = w_height / distance_to_wall;
        SDL_RenderDrawRect(renderer, &wall);
    }
}

/* Main loop */
int main() {
    SDL_Renderer *renderer;
    SDL_Window *window;

    // SDL2 Initialization
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(w_width, w_height, 0, &window, &renderer);
    
    // Initial agent position (middle of the maze)
    float agent_pos_x = maze_size / 2;
    float agent_pos_y = maze_size / 2;
    float agent_speed = 0.3;
    float agent_rotate_speed = 0.05;
    float agent_pov = 0;

    // Start ray casting simulation
    bool stop_simulation = false;
    while (!stop_simulation) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); 
        SDL_RenderClear(renderer);

        SDL_Event event;
        SDL_PollEvent(&event);

        switch (event.type) {
            case SDL_QUIT:
                stop_simulation = true;
                break;
            
            case SDL_KEYDOWN: {
                switch (event.key.keysym.scancode)
                {
                    case SDL_SCANCODE_W: {
                        float x = agent_pos_x;
                        float y = agent_pos_y;
                        y += agent_speed * sinf(agent_pov);
                        x += agent_speed * cosf(agent_pov);

                        if ((int)y > maze_size || (int)y < 0 || 
                            (int)x > maze_size || (int)x < 0) {
                            break;
                        }

                        if (maze[(int)y ][(int)x] != '#') {
                            agent_pos_y = y;
                            agent_pos_x = x;
                        }
                        break;
                    }
                    case SDL_SCANCODE_S: {
                        float x = agent_pos_x;
                        float y = agent_pos_y;
                        y -= agent_speed * sinf(agent_pov);
                        x -= agent_speed * cosf(agent_pov);

                        if ((int)y > maze_size || (int)y < 0 || 
                            (int)x > maze_size || (int)x < 0) {
                            break;
                        }

                        if (maze[(int)y][(int)x] != '#') {
                            agent_pos_y = y;
                            agent_pos_x = x;
                        }
                        break;
                    }
                    case SDL_SCANCODE_D:               
                        agent_pov += agent_rotate_speed;
                        break;
                    case SDL_SCANCODE_A:
                        agent_pov -= agent_rotate_speed;
                        break;
                    default:
                        break;
                }
            }
            default:
                break;
        }

        render_terrain(renderer, agent_pos_x, agent_pos_y, agent_pov);
        render_minimap(renderer, agent_pos_x, agent_pos_y, agent_pov);

        SDL_RenderPresent(renderer);
        SDL_Delay(FPS_delay);
    }
    
    // Destructor
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
