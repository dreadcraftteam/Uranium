// This is the main file in the Launcher project
// By DREADCRAFT, June 2025
//

#include <stdio.h>
#include <dlfcn.h>

/* Main method for launcher project */
int main()
{ 
    /* Call function for engine load */    
    loadEngine();

    return 0;
}

/* Load a file engine.so to launcher project */
void loadEngine()
{
    void *handle;
    char *error;

    void (*engine_main)();

    /* Open engine file */
    handle = dlopen("./bin/engine.so", RTLD_LAZY);

    if (!handle) 
    {
        printf("Failed to load engine.so!\n");

        return 1;
    }
    else
    {
        printf("Engine.so Loaded!\n");
    }

    /* Load main function */
    engine_main = dlsym(handle, "engine_main");

    if ((error = dlerror()) != NULL)
    {
        printf("Failed to load engine function!\n");
  
        return 1;
    }
    
    /* Call main function */
    engine_main();

    dlclose(handle);
}
