// This is the main file in the launcher project
// By DREADCRAFT
//

#include "stdio.h"

#include "dynlib.h"
#include "launcher.h"

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
    /* Open engine file */
	DynLib* engineLib;
#ifdef _WIN32
	engineLib = dynlib_open(".\\bin\\engine.dll");
#elif __linux__
	engineLib = dynlib_open("./bin/engine.so");
#else
    #error "Unsupported platform"
#endif

    /* Load main function */
	LOAD_FN(engineLib, engine_main);
    if (!engine_main)
    {
        printf("Failed to load engine_main function\n");
        dynlib_close(engineLib);
        return -1;
    }

    /* Call main function */
    engine_main();
	
    return 0;
}

