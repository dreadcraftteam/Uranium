// Console commands for Uranium (on engine)
// By DREADCRAFT
//

#include "base.h"

#include "console.h"
#include "variables.h"
#include "commands.h"

#include "../game/player.h"
#include "../game/collision.h"

/* Commands for console */
int consoleCommands(const char* commandLine)
{
    char command[256];
    char args[256] = {0};
    
    /* Console commands */
    if (sscanf(commandLine, "%255s %255[^\n]", command, args) >= 1)
    {
        /* Version */
        if (strcmp(command, "version") == 0)
        {
            Msg("Version: %s (Build %d)\n", version, build_number);
            Msg("Protocol: %s\n", protocol);
            
            return 1;
        }

        /* Exit */
        if (strcmp(command, "exit") == 0)
        {
            running = false;

            return 1;
        }

        /* Resolution <VALUExVALUE> */
        if (strcmp(command, "resolution") == 0)
        {
            if (strlen(args) > 0)
            {
                int resolution_index;
        
                if (sscanf(args, "%d", &resolution_index) == 1)
                {
                    int width, height;
            
                    switch (resolution_index)
                    {
                        case 0:
                            width = 640;
                            height = 480;
                            break;
                        case 1:
                            width = 800;
                            height = 600;
                            break;
                        case 2:
                            width = 1024;
                            height = 768;
                            break;
                        case 3:
                            width = 1280;
                            height = 960;
                            break;
                        case 4:
                            width = 1600;
                            height = 1200;
                            break;
                        default:
                            Error("Usage: resolution <NUMBER>\n");
                            Error("Resolutions (4:3): 0 - 640x480, 1 - 800x600, 2 - 1024x768, 3 - 1280x960, 4 - 1600x1200\n");
                            
                        return 1;
                    }
            
                    setWindowSize(width, height);
                    consoleSetDimensions(0, 0, width, height);
                }
                else
                {
                    Error("Usage: resolution <NUMBER>\n");
                    Error("Resolutions (4:3): 0 - 640x480, 1 - 800x600, 2 - 1024x768, 3 - 1280x960, 4 - 1600x1200\n");
                }
            }
            else
            {
                Error("Usage: resolution <VALUE>\n");
            }

            return 1;
        }

        /* Focused <VALUE> */
        if (strcmp(command, "focused") == 0)
        {
            int value;

            if (sscanf(args, "%d", &value) == 1)
            {
                focused = (value != 0);

                Msg("Focusing on window %s\n", focused ? "ON" : "OFF");
            }
            else
            {
                Error("Usage: focused <VALUE>\n");
            }

            return 1;
        }

        /* Map <MAPNAME> */
        if (strcmp(command, "map") == 0)
        {
            if (strlen(args) > 0)
            {
                char mapPath[256];
                snprintf(mapPath, sizeof(mapPath), "maps/%s.umap", args);

                FILE* mapFile = fopen(mapPath, "r");

                if (mapFile)
                {
                    // Reset this before loading the map
                    respawnPlayer();
                    enableNoClip = false;
                    enableDebugPanel = false;
                    focused = true;
                    enableFPSCounter = false;

                    fclose(mapFile);

                    char consoleMapPath[256];
                    snprintf(consoleMapPath, sizeof(consoleMapPath), "maps/%s.umap", args);

                    if (mapLoad != NULL)
                    {
                        freeMap(mapLoad);

                        mapLoad = NULL;
                    }

                    mapLoad = loadMap(consoleMapPath);

                    if (mapLoad != NULL)
                    {
                        consoleSetMapLoaded(1);
                    }
                    else
                    {
                        consoleSetMapLoaded(0);
                    }
                }
                else
                {
                    Error("Map not found!\n", mapPath);
                }
            }
            else
            {
                Error("Usage: map <MAPNAME>\n");
            }

            return 1;
        }
        
        /* Echo <MESSAGE> */
        if (strcmp(command, "echo") == 0)
        {
            if (strlen(args) > 0)
            {
                if (args[0] == '"' && args[strlen(args)-1] == '"' && strlen(args) >= 2)
                {
                    char message[256];
                    strncpy(message, args + 1, strlen(args) - 2);
                    message[strlen(args) - 2] = '\0';
                    
                    Msg("%s\n", message);
                }
                else
                {
                    Error("Message must be enclosed in quotes!\n");
                }
            }
            else
            {
                Error("Usage: echo <MESSAGE>\n");
            }

            return 1;
        }

        /* Clear */
        if (strcmp(command, "clear") == 0)
        {
            console.outputCount = 0;

            return 1;
        }

        /* Exec <CONFIGNAME> */
        if (strcmp(command, "exec") == 0)
        {
            if (strlen(args) > 0)
            {
                consoleExecuteConfigFile(args);
            }
            else
            {
                Error("Usage: exec <CONFIGNAME>\n");
            }

            return 1;
        }

        /* Quit */
        if (strcmp(command, "quit") == 0)
        {
            if (mapLoad != NULL)
            {
                freeMap(mapLoad);
                mapLoad = NULL;
                consoleSetMapLoaded(0);
                enableDebugPanel = false;
                focused = true;
            }
            else
            {
                Error("No map is currently loaded\n");
            }

            return 1;
        }

        /* Debug Panel */
        if (strcmp(command, "debug") == 0)
        {
            int value;

            if (sscanf(args, "%d", &value) == 1)
            {
                enableDebugPanel = (value != 0);
                
                if (!enableDebugPanel)
                {
                    focused = true; 
                }

                Msg("Debug panel %s\n", enableDebugPanel ? "ON" : "OFF");
            }
            else
            {
                Error("Usage: debug <VALUE>\n");
            }

            return 1;
        }

        /* FPS */
        if (strcmp(command, "fps") == 0)
        {
            int value;

            if (sscanf(args, "%d", &value) == 1)
            {
                enableFPSCounter = (value != 0);

                Msg("FPS counter %s\n", enableFPSCounter ? "ON" : "OFF");
            }
            else
            {
                Msg("Usage: fps <VALUE>\n");
            }

            return 1;
        }
    }
    
    return 0;
}
