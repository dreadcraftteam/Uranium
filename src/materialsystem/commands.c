// Console commands for Uranium (on materialsystem)
// By DREADCRAFT
//

#include "base.h"

#include "materialsystem.h"
#include "commands.h"

/* Commands for console */
int materialsystemConsoleCommands(const char* commandLine)
{
    char command[256];
    char args[256] = {0};

    if (sscanf(commandLine, "%255s %255[^\n]", command, args) >= 1)
    {
        if (strcmp(command, "mat_quality") == 0)
        {
            int value;

            if (sscanf(args, "%d", &value) == 1)
            {
                if (value >= 0 && value <= 3)
                {
                    applyTextureQuality(value);
                }
                else
                {
                    Error("Usage: mat_quality <VALUE>\n");
                    Error("0 - Low Quality, 1 - Medium Quality, 2 - High Quality, 3 - Very High Quality\n");
                }
            }
            else
            {
                Error("Usage: mat_quality <VALUE>\n");
            }

            return 1;
        }            
    }
    
    return 0; 
}
