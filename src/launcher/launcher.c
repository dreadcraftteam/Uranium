// This is the main file in the launcher project
// By DREADCRAFT
//

#include "stdio.h"

#include "dynlib.h"
#include "launcher.h"

/* Main method for launcher project */
int main(void)
{
    loadEngineFile();

    return 0;
}

/* Load a file engine.so to launcher */
void loadEngineFile(void)
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
	LOAD_FN(engineLib, engineMain);
    if (!engineMain)
    {
        printf("Failed to load engineMain function\n");
        dynlib_close(engineLib);

        return;
    }

    /* Call main function */
    engineMain();
	
    return 0;
}
