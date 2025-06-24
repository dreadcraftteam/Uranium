// This is the main file in the Launcher project.
// By DREADCRAFT, June 2025
//

#include <stdio.h>

#include <dlfcn.h>

int main()
{
    void *handle;
    void (*engine_main)();
    char *error;

    // Open .SO file...
    handle = dlopen("./engine.so", RTLD_LAZY);
    if (!handle) 
    {
        printf("Failed to load .SO file! Programmer is Idiot!");
        return 1;
    }

    // Load 'engine_main'...
    engine_main = dlsym(handle, "engine_main");
    if ((error = dlerror()) != NULL)
    {
        printf("Failed to load 'engine_main'! Programmer is Idiot!");
        return 1;
    }

    // Call functions from .SO file...
    engine_main();

    // MEMORY LEAK EAAEAEAAAA!!@!!!11!!
    dlclose(handle);
   
    return 0;
}
