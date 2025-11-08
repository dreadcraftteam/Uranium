// Console commands for Uranium (on engine)
// By DREADCRAFT
//

#include "base.h"

#include "console.h"
#include "variables.h"
#include "commands.h"

#include "../game/player.h"
#include "../game/collision.h"

KeyBind keyBinds[MAX_BINDS];
int bindCount = 0;

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

        /* Resolution <VALUE> */
        if (strcmp(command, "resolution") == 0)
        {
            if (strlen(args) > 0)
            {
                int resolutionIndex;
        
                if (sscanf(args, "%d", &resolutionIndex) == 1)
                {
                    int width; 
                    int height;
            
                    switch (resolutionIndex)
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

        /* FPS <VALUE> */
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

        /* Bind <KEY> <COMMANDNAME> */
        if (strcmp(command, "bind") == 0)
        {
            char keyName[64];
            char commandName[128];

            if (sscanf(args, "%63s %127[^\n]", keyName, commandName) == 2)
            {
                int key = 0;

                if (strlen(keyName) == 1)
                {
                    key = toupper(keyName[0]);
                }
                else if (strcasecmp(keyName, "F1") == 0)
                {
                    key = INPUT_KEY_F1;
                }
                else if (strcasecmp(keyName, "F2") == 0)
                {
                    key = INPUT_KEY_F2;
                }
                else if (strcasecmp(keyName, "F3") == 0)
                {
                    key = INPUT_KEY_F3;
                }
                else if (strcasecmp(keyName, "F4") == 0)
                {
                    key = INPUT_KEY_F4;
                }
                else if (strcasecmp(keyName, "F5") == 0)
                {
                    key = INPUT_KEY_F5;
                }
                else if (strcasecmp(keyName, "F6") == 0)
                {
                    key = INPUT_KEY_F6;
                }
                else if (strcasecmp(keyName, "F7") == 0)
                {
                    key = INPUT_KEY_F7;
                }
                else if (strcasecmp(keyName, "F8") == 0)
                {
                    key = INPUT_KEY_F8;
                }
                else if (strcasecmp(keyName, "F9") == 0)
                {
                    key = INPUT_KEY_F9;
                }
                else if (strcasecmp(keyName, "F10") == 0)
                {
                    key = INPUT_KEY_F10;
                }
                else if (strcasecmp(keyName, "F11") == 0)
                {
                    key = INPUT_KEY_F11;
                }
                else if (strcasecmp(keyName, "F12") == 0)
                {
                    key = INPUT_KEY_F12;
                }
                if (strcasecmp(keyName, "A") == 0) 
                {
                    key = INPUT_KEY_A;
                } 
                else if (strcasecmp(keyName, "B") == 0) 
                {
                    key = INPUT_KEY_B;
                } 
                else if (strcasecmp(keyName, "C") == 0) 
                {
                    key = INPUT_KEY_C;
                } 
                else if (strcasecmp(keyName, "D") == 0) 
                {
                    key = INPUT_KEY_D;
                } 
                else if (strcasecmp(keyName, "E") == 0) 
                {
                    key = INPUT_KEY_E;
                } 
                else if (strcasecmp(keyName, "F") == 0) 
                {
                    key = INPUT_KEY_F;
                } 
                else if (strcasecmp(keyName, "G") == 0) 
                {
                    key = INPUT_KEY_G;
                } 
                else if (strcasecmp(keyName, "H") == 0) 
                {
                    key = INPUT_KEY_H;
                } 
                else if (strcasecmp(keyName, "I") == 0) 
                {
                    key = INPUT_KEY_I;
                } 
                else if (strcasecmp(keyName, "J") == 0) 
                {
                    key = INPUT_KEY_J;
                } 
                else if (strcasecmp(keyName, "K") == 0) 
                {
                    key = INPUT_KEY_K;
                } 
                else if (strcasecmp(keyName, "L") == 0) 
                {
                    key = INPUT_KEY_L;
                } 
                else if (strcasecmp(keyName, "M") == 0) 
                {
                    key = INPUT_KEY_M;
                } 
                else if (strcasecmp(keyName, "N") == 0) 
                {
                    key = INPUT_KEY_N;
                } 
                else if (strcasecmp(keyName, "O") == 0) 
                {
                    key = INPUT_KEY_O;
                } 
                else if (strcasecmp(keyName, "P") == 0) 
                {
                    key = INPUT_KEY_P;
                } 
                else if (strcasecmp(keyName, "Q") == 0) 
                {
                    key = INPUT_KEY_Q;
                } 
                else if (strcasecmp(keyName, "R") == 0) 
                {
                    key = INPUT_KEY_R;
                } 
                else if (strcasecmp(keyName, "S") == 0) 
                {
                    key = INPUT_KEY_S;
                } 
                else if (strcasecmp(keyName, "T") == 0) 
                {
                    key = INPUT_KEY_T;
                } 
                else if (strcasecmp(keyName, "U") == 0) 
                {
                    key = INPUT_KEY_U;
                } 
                else if (strcasecmp(keyName, "V") == 0) 
                {
                    key = INPUT_KEY_V;
                } 
                else if (strcasecmp(keyName, "W") == 0) 
                {
                    key = INPUT_KEY_W;
                } 
                else if (strcasecmp(keyName, "X") == 0) 
                {
                    key = INPUT_KEY_X;
                } 
                else if (strcasecmp(keyName, "Y") == 0) 
                {
                    key = INPUT_KEY_Y;
                } 
                else if (strcasecmp(keyName, "Z") == 0) 
                {
                    key = INPUT_KEY_Z;
                } 
                else 
                {
                    Error("Unknown key: %s\n", keyName);

                    return 1;
                }

                if (bindCount < MAX_BINDS)
                {
                    keyBinds[bindCount].key = key;
                    strncpy(keyBinds[bindCount].command, commandName, sizeof(keyBinds[bindCount].command)-1);
                    bindCount++;
                }
                else
                {
                    Warning("Bind limit reached!\n");
                }
            }
            else
            {
                Error("Usage: bind <KEY> <COMMANDNAME>\n");
            }

            return 1;
        }

        /* Unbind <KEY> */
        if (strcmp(command, "unbind") == 0)
        {
            char keyName[64];

            if (sscanf(args, "%63s", keyName) == 1)
            {
                int key = 0;

                if (strlen(keyName) == 1)
                {
                    switch (toupper(keyName[0]))
                    {
                        case 'A': 
                            key = INPUT_KEY_A; 
                            break;
                        case 'B': 
                            key = INPUT_KEY_B; 
                            break;
                        case 'C': 
                            key = INPUT_KEY_C; 
                            break;
                        case 'D': 
                            key = INPUT_KEY_D; 
                            break;
                        case 'E': 
                            key = INPUT_KEY_E; 
                            break;
                        case 'F': 
                            key = INPUT_KEY_F; 
                            break;
                        case 'G': 
                            key = INPUT_KEY_G; 
                            break;
                        case 'H': 
                            key = INPUT_KEY_H; 
                            break;
                        case 'I': 
                            key = INPUT_KEY_I; 
                            break;
                        case 'J': 
                            key = INPUT_KEY_J; 
                            break;
                        case 'K': 
                            key = INPUT_KEY_K; 
                            break;
                        case 'L': 
                            key = INPUT_KEY_L; 
                            break;
                        case 'M': key = 
                            INPUT_KEY_M; 
                            break;
                        case 'N': 
                            key = INPUT_KEY_N; 
                            break;
                        case 'O': 
                            key = INPUT_KEY_O; 
                            break;
                        case 'P': 
                            key = INPUT_KEY_P; 
                            break;
                        case 'Q':
                            key = INPUT_KEY_Q; 
                            break;
                        case 'R': 
                            key = INPUT_KEY_R; 
                            break;
                        case 'S': 
                            key = INPUT_KEY_S; 
                            break;
                        case 'T': 
                            key = INPUT_KEY_T; 
                            break;
                        case 'U': 
                            key = INPUT_KEY_U; 
                            break;
                        case 'V': 
                            key = INPUT_KEY_V; 
                            break;
                        case 'W': 
                            key = INPUT_KEY_W; 
                            break;
                        case 'X': 
                            key = INPUT_KEY_X; 
                            break;
                        case 'Y': 
                            key = INPUT_KEY_Y;
                            break;
                        case 'Z': 
                            key = INPUT_KEY_Z; 
                            break;
                        default:
                            key = 0;
                    }
                }
                if (strlen(keyName) == 1)
                {
                    key = toupper(keyName[0]);
                }
                else if (strcasecmp(keyName, "F1") == 0)
                {
                    key = INPUT_KEY_F1;
                }
                else if (strcasecmp(keyName, "F2") == 0)
                {
                    key = INPUT_KEY_F2;
                }
                else if (strcasecmp(keyName, "F3") == 0)
                {
                    key = INPUT_KEY_F3;
                }
                else if (strcasecmp(keyName, "F4") == 0)
                {
                    key = INPUT_KEY_F4;
                }
                else if (strcasecmp(keyName, "F5") == 0)
                {
                    key = INPUT_KEY_F5;
                }
                else if (strcasecmp(keyName, "F6") == 0)
                {
                    key = INPUT_KEY_F6;
                }
                else if (strcasecmp(keyName, "F7") == 0)
                {
                    key = INPUT_KEY_F7;
                }
                else if (strcasecmp(keyName, "F8") == 0)
                {
                    key = INPUT_KEY_F8;
                }
                else if (strcasecmp(keyName, "F9") == 0)
                {
                    key = INPUT_KEY_F9;
                }
                else if (strcasecmp(keyName, "F10") == 0)
                {
                    key = INPUT_KEY_F10;
                }
                else if (strcasecmp(keyName, "F11") == 0)
                {
                    key = INPUT_KEY_F11;
                }
                else if (strcasecmp(keyName, "F12") == 0)
                {
                    key = INPUT_KEY_F12;
                }

                if (key == 0)
                {
                    Error("Unknown key: %s\n", keyName);

                    return 1;
                }

                bool found = false;
                for (int i = 0; i < bindCount; i++)
                {
                    if (keyBinds[i].key == key)
                    {
                        for (int j = i; j < bindCount - 1; j++)
                        {
                            keyBinds[j] = keyBinds[j + 1];
                        }
                        bindCount--;
                        found = true;
                        break;
                    }
                }
            }
            else
            {
                Error("Usage: unbind <KEY>\n");
            }

            return 1;
        }

        /* Unbind All */
        if (strcmp(command, "unbindall") == 0)
        {
            bindCount = 0;

            return 1;
        }
    }
    
    return 0;
}
