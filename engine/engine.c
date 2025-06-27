// This is the main file in the Engine project
// By DREADCRAFT, June 2025
//

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "engine_variables.h"

#include <SDL2/SDL.h>
#include <glad/glad.h>

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

    SDL_Window* frame = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (!frame)
    {
        SDL_Quit();
       
        return 1;
    }

    /* OpenGL Attributes */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    
    /* OpenGL Context */
    SDL_GLContext glContext = SDL_GL_CreateContext(frame);
    if (!glContext)
    {
        SDL_DestroyWindow(frame);
        SDL_Quit();
     
        return 1;
    }

    /* GLAD initialization */
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(frame);
        SDL_Quit();

        return 1;
    }

    /* OpenGL VSYNC */
    SDL_GL_SetSwapInterval(1);

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

        /* Clear Color and Depth Buffer */
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        /* OpenGL Code Here */

        /* Swap Buffers */
        SDL_GL_SwapWindow(frame);
    }

    /* Destroy OpenGL Context */
    SDL_GL_DeleteContext(glContext); 
   
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
