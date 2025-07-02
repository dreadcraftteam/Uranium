// This is the main file in the Engine project
// By DREADCRAFT, June 2025
//

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "engine_variables.h"

#include <GL/glew.h>
#include <GL/glu.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

/* Main method for engine project */
int engine_main()
{
    /* Call function for game load */
    load_game();
    
    /* Window on SDL (Simple DirectMedia Layer) */
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        return 1;
    }

    /* Set OpenGL attributes before creating the window */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window* frame = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (!frame)
    {
        SDL_Quit();
       
        return 1;
    }

    /* Create OpenGL context */
    SDL_GLContext glContext = SDL_GL_CreateContext(frame);
    if (!glContext)
    {
        fprintf(stderr, "OpenGL context creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(frame);
        SDL_Quit();
        return 1;
    }

    /* Initialize GLEW */
    glewExperimental = GL_TRUE;

    GLenum glewError = glewInit();

    if (glewError != GLEW_OK)
    {
        fprintf(stderr, "GLEW initialization failed: %s\n", glewGetErrorString(glewError));
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(frame);
        SDL_Quit();
        return 1;
    }

    /* Enable VSYNC */
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

        /* Clear the screen */
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(gl_red, gl_green, gl_blue, gl_alpha);
        
	/* Here you would add your rendering code */

        /* Swap buffers */
        SDL_GL_SwapWindow(frame);

    }
    
    SDL_GL_DeleteContext(glContext);

    SDL_DestroyWindow(frame);
    SDL_Quit();

    return 0;
}

/* Load a file game.so to engine project */
void load_game()
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
