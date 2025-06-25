// This is the main file in the Engine project
// By DREADCRAFT, June 2025
//

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "engine_variables.h"

#include <SDL2/SDL.h>

/* Main method for engine project */
int engine_main()
{
    /* Call function for game load */
    loadGame();
    
    /* Window on SDL (Simple DirectMedia Layer) */
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        return 1;
    }

    SDL_Window* frame = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!frame)
    {
        SDL_Quit();
       
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(frame, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        SDL_DestroyWindow(frame);
        SDL_Quit();
   
        return 1;
    }

    SDL_Event event;
    
    /* Cycle */
    while (running)
    {
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            
            /* Close the window */
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {
                    running = false;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(frame);
    SDL_Quit();

    return 0;
}

/* Load a file game.so to engine project */
void loadGame()
{
    void *handle;
    char *error;

    void (*game_main)();

    /* Open game file */
    handle = dlopen("./bin/game.so", RTLD_LAZY);

    if (!handle) 
    {
        printf("Failed to load game.so!\n");

        return 1;
    }
    else
    {
        printf("Game.so Loaded!\n");
    }

    /* Load main function */
    game_main = dlsym(handle, "game_main");

    if ((error = dlerror()) != NULL)
    {
        printf("Failed to load game function!\n");
  
        return 1;
    }
    
    /* Call main function */
    game_main();

    dlclose(handle);
}
