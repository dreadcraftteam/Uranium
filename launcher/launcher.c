// This is the main file in the launcher project
// By DREADCRAFT
//

#include "stdio.h"
#include "dlfcn.h"

/* Main method for launcher project */
int main()
{ 
    /* Call function for engine load */    
    loadEngineFile();

    return 0;
}

/* Load a file engine.so to launcher */
void loadEngineFile()
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
