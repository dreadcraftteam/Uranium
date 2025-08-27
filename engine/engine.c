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

#ifdef USE_GAMEUI
#include "gameui/pause.h"
#endif

#include "umap.h"
#include "variables.h"

int renderMode = 2;

bool running = true;

/* Main method for engine project */
int engine_main(int argc, char* argv[])
{
    /* Load game title */
    loadGameTitle();

    /* GLFW initialization */
    glfwInit();
    if (!glfwInit()) 
    {
        Error("Failed to initialize GLFW\n");
        
        return -1;
    }

    /* Configuring GLFW */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    /* Creating window */
    GLFWwindow* frame = glfwCreateWindow(width, height, title, NULL, NULL);

    /* Centerize window on the screen */
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    if (primaryMonitor)
    {
        const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
        if (videoMode)
        {
            int monitorX, monitorY;
            glfwGetMonitorPos(primaryMonitor, &monitorX, &monitorY);
            
            int windowPosX = monitorX + (videoMode->width - width) / 2;
            int windowPosY = monitorY + (videoMode->height - height) / 2;
            
            glfwSetWindowPos(frame, windowPosX, windowPosY);
        }
    }

    glfwMakeContextCurrent(frame);

    /* OpenGL initialization */
    glewInit();

    /* Enabling Depth testing, Lighting and etc... */
    glEnable(GL_DEPTH_TEST);

    /* Lighting */
    glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    /* Load game file */
	DynLib* gameLib;

    #ifdef _WIN32
        gameLib = dynlib_open(".\\bin\\game.dll");
    #elif __linux__
        gameLib = dynlib_open("./bin/game.so");
    #else
        #error "Unsupported platform"
    #endif

    /* Load the game necessary funcs */
	LOAD_FN(gameLib, gameInit);
    LOAD_FN(gameLib, gameUpdate);
	LOAD_FN(gameLib, gameShutdown);

    /* Audio system initialization */
    audio = audioSystemCreate();

    /* Game initialization */
    gameInit();

    /* Map loading */
    mapLoad = loadMap("maps/main.umap");

    /* Configuring Map System */
    int brushCount = countBrushes(mapLoad);

    Entity** brushes = getBrushArray(mapLoad);
	BSPNode* bspRoot = buildBSP(brushes, brushCount, 0);
    
    free(brushes);

    /* Input sytem initialization */
    inputSystemInit(frame);

    /* Main cycle */
    while (running)
    {
        /* Configuring OpenGL */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(glRed, glGreen, glBlue, glAlpha);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float ratio = width / (float) height;
        glFrustum(-ratio * 1.0f, -ratio * -1.0f, -1.0f, 1.0f, 2.0f, 500.0f);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        /* Updating input system */
        inputSystemUpdate();

#ifdef USE_GAMEUI
        updatePauseMenu(frame);
#endif 

#ifdef USE_GAMEUI
        if (!isGamePaused()) {
            /* Basic input commands */
            basicInputHandle(frame);

            /* Game updating */
            gameUpdate(frame);
            
            /* Map system stuff */
            float cameraPos[3] = { cameraPos[0], cameraPos[1], cameraPos[2] };
            setupLights(mapLoad);
            renderMap(bspRoot, cameraPos);
        }
#else 
        /* Basic input commands */
        basicInputHandle(frame);

        /* Game updating */
        gameUpdate(frame);
            
        /* Map system stuff */
        float cameraPos[3] = { cameraPos[0], cameraPos[1], cameraPos[2] };
        setupLights(mapLoad);
        renderMap(bspRoot, cameraPos);
#endif 

#ifdef USE_GAMEUI
        renderPauseMenu(frame);
#endif

        glfwSwapBuffers(frame);

        glfwPollEvents();
    }

    /* Game shutdown */
    gameShutdown();

    /* Audio system shutdown */
    audioSystemDestroy(audio);

    /* GLFW shutdown */
    glfwDestroyWindow(frame);
    glfwTerminate();

    return 0;
}

/* This is very basic and low-level input */
void basicInputHandle(GLFWwindow* frame)
{
#ifdef USE_GAMEUI
    /* There's nothing here, as there's already a pause on escape */
#else 
    /* Close the window */
    if(KEY_PRESSED(INPUT_KEY_ESCAPE))
    {
        running = false;
        
        return;
    }
#endif

    /* Changes render mode */
    if(KEY_PRESSED(INPUT_KEY_F1))
    {
        renderMode = (renderMode + 1) % 3;

        Msg("Render Mode Value: %d\n", renderMode);
    }
}

/* Load title from info.txt file */
void loadGameTitle() 
{
    /* If you change this, you will be fired */

    FILE* file = fopen("./info.txt", "r");
    if (!file) 
    {
        Error("Could not open info.txt!\n");
        
        return;
    }

    char line[256];

    while (fgets(line, sizeof(line), file)) 
    {
        if (strstr(line, "title")) 
        {
            char* start = strchr(line, '"');
            if (start) {
                char* end = strchr(start + 1, '"');
                if (end) 
                {
                    *end = '\0';
                    title = strdup(start + 1);
                }
            }
        }
    }

    fclose(file);
}
