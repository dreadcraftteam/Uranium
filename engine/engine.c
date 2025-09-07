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
#include "textrenderer.h"

int renderMode = 2;

bool running = true;

/* Main method for engine project */
int engine_main(int argc, char* argv[])
{
    /* Load game title */
    loadGameTitle();

    /* GLFW initialization */
    glfwInit();

    /* Configuring GLFW */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

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

    /* Load game file */
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

    /* Enabling Depth testing, Lighting and etc... */
    glEnable(GL_DEPTH_TEST);

    /* Lighting */
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);

    /* Set screen dimensions for text rendering */
    setScreenDimensions(width, height);

    /* Audio system initialization */
    audio = audioSystemCreate();

    /* Initialize Text Rendering */
    textRendererInit();

    /* Game initialization */
    gameInit();

    /* Map loading */
    mapLoad = loadMap("maps/main.umap");

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

        /* Basic input commands */
        basicInputHandle(frame);

        /* Game rendering */
        gameRender(frame);             

        /* Map system stuff */
        float cameraPos[3] = { cameraPos[0], cameraPos[1], cameraPos[2] };
        setupLights(mapLoad);
        renderMap(mapLoad, cameraPos);

        /* Begining text rendering */
        beginTextRendering();

        /* Game updating */
        gameUpdate();

        /* End text rendering */
        endTextRendering();

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
    /* Close the window */
    if(KEY_PRESSED(INPUT_KEY_ESCAPE))
    {
        running = false;
        
        return;
    }

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

    FILE* file = fopen("./config/info.txt", "r");
    if (!file) 
    {
        Error("Could not open info.txt!\n");
        
        return;
    }

    char line[256];

    while (fgets(line, sizeof(line), file)) 
    {
        char* trimmed_line = line;
        while (*trimmed_line == ' ' || *trimmed_line == '\t') trimmed_line++;
        
        if (strstr(trimmed_line, "title")) 
        {
            char* start = strchr(trimmed_line, '"');
            if (start) {
                char* end = strchr(start + 1, '"');
                if (end) 
                {
                    *end = '\0';
                    title = strdup(start + 1);
                    break;
                }
            }
        }
    }

    fclose(file);
}

