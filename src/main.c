// The main file for the Uranium Engine project (Don't write trash here!)
// By DREADCRAFT, June 2025
//

#include <stdio.h>
#include <stdbool.h>

#include <SDL3/SDL.h>

// Main method.
int main(int argc, char* argv[]) 
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* frame = SDL_CreateWindow("Uranium", 800, 600, SDL_WINDOW_OPENGL);

    if (!frame) 
    {
        printf("Failed to create window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(frame, NULL);

    if (!renderer) 
    {
        printf("SDL Renderer inititalization Failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(frame);
        SDL_Quit();
        return 1;
    }

    SDL_Event event;
    int running = 1;

    while (running) 
    {
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_EVENT_QUIT) 
            {
                running = 0;
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