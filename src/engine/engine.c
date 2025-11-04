// This is the main file in the engine project 
// By DREADCRAFT
//

#include "base.h"

#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

#include "audio/audio.h"
#include "defines.h"
#include "dynlib.h"
#include "engine.h"
#include "umap.h"
#include "variables.h"
#include "console.h"
#include "gameui_engine.h"
#include "commands.h"

/* General variables for engine */
char* title = NULL;

bool running = true;
bool focused = true;

int enableDebugPanel = 0;

/* Player variables that are required to be in the engine */
float playerX = 0.0f;
float playerY = 0.0f;
float playerZ = 5.0f;

float cameraX = 85.0f;
float cameraY = 0.0f;
float cameraZ = 0.0f;

double mouseX;
double mouseY;

bool enableNoClip = false;

/* Window stuff */
static GLFWwindow* mainFrame = NULL;

int screenWidth = 800;
int screenHeight = 600;

/* Main method for engine project */
int engineMain(int argc, char* argv[])
{
    /* Load game name */
    loadGameInfo();
    
    /* GLFW initialization */
    glfwInit();

    /* Configuring GLFW */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

    /* Creating window */
    GLFWwindow* frame = glfwCreateWindow(screenWidth, screenHeight, title, NULL, NULL);
    mainFrame = frame;

    /* Centerize window on the screen */
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    if (primaryMonitor)
    {
        const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
        
        if (videoMode)
        {
            int monitorX, monitorY;
            glfwGetMonitorPos(primaryMonitor, &monitorX, &monitorY);
            
            int windowPosX = monitorX + (videoMode->width - screenWidth) / 2;
            int windowPosY = monitorY + (videoMode->height - screenHeight) / 2;
            
            glfwSetWindowPos(frame, windowPosX, windowPosY);
        }
    }

    /* Creating Context */
    glfwMakeContextCurrent(frame);

    /* Set input callbacks for console system */
    glfwSetKeyCallback(frame, consoleKeyCallback);
    glfwSetCharCallback(frame, consoleCharCallback);

    /* Loading libs! */

    /* Load game */
    DynLib* gameLib;

    #ifdef _WIN32
        gameLib = dynlib_open(".\\bin\\game.dll");
    #elif __linux__
        gameLib = dynlib_open("./bin/game.so");
    #endif

    /* Load the game necessary funcs */
    LOAD_FN(gameLib, gameInit);
    LOAD_FN(gameLib, gameRender);
    LOAD_FN(gameLib, gameUpdate);
    LOAD_FN(gameLib, gameShutdown);
    LOAD_FN(gameLib, gameInputHandle);
    LOAD_FN(gameLib, Game_GameUI_Init);
    LOAD_FN(gameLib, Game_GameUI_Update);
    LOAD_FN(gameLib, Game_GameUI_Shutdown);

    /* Load gameui file */
    DynLib* gameuiLib;

    #ifdef _WIN32
        gameuiLib = dynlib_open(".\\bin\\gameui.dll");
    #elif __linux__
        gameuiLib = dynlib_open("./bin/gameui.so");
    #endif

    /* Load the gameui necessary funcs */
    LOAD_FN(gameuiLib, setScreenDimensions);
    LOAD_FN(gameuiLib, beginGameUIRendering);
    LOAD_FN(gameuiLib, endGameUIRendering);
    LOAD_FN(gameuiLib, gameuiInit);
    LOAD_FN(gameuiLib, gameuiUpdate);
    LOAD_FN(gameuiLib, gameuiShutdown);
    
    /* Load inputsystem file */
    DynLib* inputsystemLib;

    #ifdef _WIN32
        inputsystemLib = dynlib_open(".\\bin\\inputsystem.dll");
    #elif __linux__
        inputsystemLib = dynlib_open("./bin/inputsystem.so");
    #endif

    /* Load the inputsystem necessary funcs */
    LOAD_FN(inputsystemLib, inputSystemInit);
    LOAD_FN(inputsystemLib, inputSystemUpdate);
    LOAD_FN(inputsystemLib, inputSystemShutdown);

    /* Load materialsystem file */
    DynLib* materialsystemLib;

    #ifdef _WIN32
        materialsystemLib = dynlib_open(".\\bin\\materialsystem.dll");
    #elif __linux__
        materialsystemLib = dynlib_open("./bin/materialsystem.so");
    #endif

    /* Load the materialsystem necessary funcs */
    LOAD_FN(materialsystemLib, materialInit);
    LOAD_FN(materialsystemLib, materialShutdown);

    /* OpenGL initialization */
    glewInit();

    /* Enabling Depth testing, Lighting and etc... */
    glEnable(GL_DEPTH_TEST);

    /* Lighting */
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    /* Set screen dimensions for GameUI */
    setScreenDimensions(screenWidth, screenHeight);

    /* Audio system initialization */
    audio = audioSystemCreate();

    /* Initializing GameUI */
    gameuiInit();

    /* Initializing console */
    consoleInit();

    /* Initializing Game */
    gameInit();

    /* Initializing Inputsystem */
    inputSystemInit(frame);

    /* Initializing Materialsystem */
    materialInit();

    /* Loader for gameui menus on engine */
    Engine_GameUI_Init();

    /* Loader for gameui menus on game */
    Game_GameUI_Init();

    /* Load autoexec.cfg on startup */
    consoleExecuteConfigFile(FILE_AUTOEXEC);

    /* Funny dates and messages initialization */
    funnyDates();
 
    /* Main cycle */
    while (running)
    {
        /* Begin game rendering */
        beginGameRendering();

        /* Everything related to game and engine should be here! */
        
        /* Updating input system */
        inputSystemUpdate();

        /* Game rendering */
        gameRender(); 

        /* Basic input commands */
        baseInputHandle(frame);

        if (!consoleIsOpen())
        {
            /* Input handle for game */
            gameInputHandle(frame);
        }

        /* Updating the game */
        gameUpdate(frame);

        /* Update console */
        consoleUpdate();   

        /* Map system stuff */
        setupLights(mapLoad);
        
        float cameraPos[3] = {cameraX, cameraY, cameraZ};
		renderMap(mapLoad, cameraPos);

        /* End game rendering */
        endGameRendering();

        /* Begin GameUI rendering */
        beginGameUIRendering();

        gameuiUpdate();

        /* Loader for gameui menus on engine */
        Engine_GameUI_Update(frame);

        /* Loader for gameui menus on game */
        Game_GameUI_Update(frame);

        /* Draw console (on top of everything) */
        consoleDraw();

        /* End GameUI rendering */
        endGameUIRendering();

        glfwSwapBuffers(frame);

        glfwPollEvents();
    }

    /* Game shutdown */
    gameShutdown();

    /* Audio system shutdown */
    audioSystemDestroy(audio);

    /* GameUI shutdown */
    gameuiShutdown();

    /* Input system shutdown */
    inputSystemShutdown();
    
    /* Shutdown texture system */
    materialShutdown();

    /* Loader for gameui menus on engine */
    Engine_GameUI_Shutdown();

    /* Loader for gameui menus on game */
    Game_GameUI_Shutdown();

    /* GLFW shutdown */
    glfwDestroyWindow(frame);
    glfwTerminate();

    return 0;
}

/* This is very basic and low-level input */
void baseInputHandle(GLFWwindow* frame)
{
    if (KEY_PRESSED(INPUT_KEY_F1))
    {
        consoleToggle();

        return;
    }
}

/* Load title and version from info.txt file */
void loadGameInfo(void) 
{
    FILE* file = fopen(FILE_INFO, "r");
    if (!file) 
    {
        Error("Could not open info.txt!\n");
        
        return;
    }

    char line[256];
    bool titleFound = false;

    while (fgets(line, sizeof(line), file)) 
    {
        char* trimmed_line = line;
        while (*trimmed_line == ' ' || *trimmed_line == '\t') trimmed_line++;
        
        /* Game title */
        if (!titleFound && strstr(trimmed_line, "title")) 
        {
            char* start = strchr(trimmed_line, '"');
            if (start) 
            {
                char* end = strchr(start + 1, '"');
                if (end) 
                {
                    *end = '\0';
                    title = strdup(start + 1);
                    titleFound = true;
                }
            }
        }

        /* If they found, break early */
        if (titleFound)
            break;
    }

    fclose(file);
    
    if (!titleFound)
    {
        Error("Game title not found in info.txt!\n");
        
        return;
    }
}

/* Begin game rendering and configuring OpenGL */
void beginGameRendering(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Set viewport to current window size
    glViewport(0, 0, screenWidth, screenHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float ratio = screenWidth / (float) screenHeight;
    glFrustum(-ratio * 1.0f, ratio * 1.0f, -1.0f, 1.0f, 2.0f, 500.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/* End game rendering */
void endGameRendering(void)
{
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

/* Window resize callback */
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    screenWidth = width;
    screenHeight = height;
    
    // Update viewport
    glViewport(0, 0, screenWidth, screenHeight);
    
    setScreenDimensions(screenWidth, screenHeight);
    consoleSetDimensions(0, 0, screenWidth, screenHeight);
    
    consoleUpdate();
}

/* Set window size */
void setWindowSize(int width, int height)
{
    if (width <= 0 || height <= 0) 
    {
        return;
    }
    
    screenWidth = width;
    screenHeight = height;
    
    if (mainFrame) 
    {
        glfwSetWindowSize(mainFrame, screenWidth, screenHeight);
        
        // Force viewport update
        glViewport(0, 0, screenWidth, screenHeight);
        
        // Center window after resize
        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        if (primaryMonitor)
        {
            const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
            if (videoMode)
            {
                int monitorX, monitorY;
                glfwGetMonitorPos(primaryMonitor, &monitorX, &monitorY);
                
                int windowPosX = monitorX + (videoMode->width - screenWidth) / 2;
                int windowPosY = monitorY + (videoMode->height - screenHeight) / 2;
                
                glfwSetWindowPos(mainFrame, windowPosX, windowPosY);
            }
        }
        
        setScreenDimensions(screenWidth, screenHeight);
        
        consoleSetDimensions(0, 0, screenWidth, screenHeight);
    }
}

/* Funny dates and messages */
void funnyDates(void)
{
    // time.h already included in base.h
    time_t t = time(NULL);
    struct tm *currentTime = localtime(&t);

    if (currentTime->tm_mon == 11 && currentTime->tm_mday == 12) 
    {
        Msg("Happy birthday DREADCRAFT!\n");
    }
    else if (currentTime->tm_mon == 11 && currentTime->tm_mday == 31)
    {
        Msg("Happy new year!\n");
    }
    else if (currentTime->tm_mon == 5 && currentTime->tm_mday == 24)
    {
        Msg("Happy birthday Uranium (engine)!\n");
    }
}
