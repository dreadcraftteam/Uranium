// This is the main file in the Launcher project
// By DREADCRAFT, June 2025
//

#include <stdio.h>
#include <dlfcn.h>

int main()
{ 
    void *handle;
    char *error;

    void (*engine_main)();

    handle = dlopen("./bin/engine.so", RTLD_LAZY);

    if (!handle) 
    {
        printf("Failed to load engine.so!\n");

        return 1;
    }

    engine_main = dlsym(handle, "engine_main");

    if ((error = dlerror()) != NULL)
    {
        printf("Failed to load engine function!\n");
  
        return 1;
    }

    engine_main();

    dlclose(handle);
   
    return 0;
}
