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
