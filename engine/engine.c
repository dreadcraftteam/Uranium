// This is the main file in the engine project
// By DREADCRAFT
//

#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"
#include "dlfcn.h"
#include "string.h"
#include "math.h"

#include "GL/glew.h"
#include "GL/gl.h"

#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

#include "engine_camera.h"
#include "engine_ground.h"
#include "engine_variables.h"

/* Main method for engine project */
int engine_main(int argc, char* argv[])
{
    /* Load game title */
    loadGameTitle();

    /* Declaring functions from game.so */
    void *load_handle;
    char *load_error;

    void (*game_init)();
    void (*game_render)();
    void (*game_shutdown)();

    /* GLFW initialization */
    glfwInit();

    if (!glfwInit()) 
    {
        printf("GLFW initialization failed!\n");
        
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    /* Creating window */
    GLFWwindow* frame = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!frame) 
    {
        printf("Window creation failed!\n");
        glfwTerminate();

        return -1;
    }

    glfwMakeContextCurrent(frame);

    /* OpenGL initialization */
    glewExperimental = GL_TRUE;
    glewInit();
    
    if (glewInit() != GLEW_OK) 
    {
        printf("OpenGL initialization failed!\n");
        
        return -1;
    }
    
    glEnable(GL_DEPTH_TEST);

    /* Open game.so file and read information... */
    load_handle = dlopen("./bin/game.so", RTLD_LAZY);
    
    game_init = dlsym(load_handle, "game_init");
    game_render = dlsym(load_handle, "game_render");
    game_shutdown = dlsym(load_handle, "game_shutdown");

    /* Checking load_handle */
    if (!load_handle)
    {
        printf("Failed to load game.so!\n");

        return -1;
    }

    /* Checking functions */
    if ((load_error = dlerror()) != NULL)
    {
        printf("Failed to load game.so functions!\n");

        return -1;
    }

    /* Game initialization */
    game_init();

    while (!glfwWindowShouldClose(frame))
    {
        /* Configuring OpenGL */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(gl_red, gl_green, gl_blue, gl_alpha);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float ratio = width / (float) height;
        glFrustum(-ratio * 1.0f, -ratio * -1.0f, -1.0f, 1.0f, 2.0f, 100.0f);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        glPushMatrix();

            /* The camera should always be created first! */
            camera();

            drawGround();

            /* Render the game */
            game_render();

        glPopMatrix();

        /* Keyboard and mouse input */
        handleInput(frame);

        updatePlayerPosition(x, y, z);
    
        glfwSwapBuffers(frame);

        glfwPollEvents();
    }
    
    /* Game shutdown */
    game_shutdown();

    glfwDestroyWindow(frame);
    glfwTerminate();
    
    return 0;
}

/* Keyboard and mouse input */
void handleInput(GLFWwindow* frame)
{
    float angle = -camera_z / 180 * M_PI;
    float speed = 0;

    int centerX = width / 2;
    int centerY = height / 2;   

    glfwSetInputMode(frame, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    cursor_hidden = true;

    if (glfwGetKey(frame, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(frame, GLFW_TRUE);
        
        return;
    }
    if (glfwGetKey(frame, GLFW_KEY_UP) == GLFW_PRESS)
    {
        camera_x = (camera_x + 1) > 180 ? 180 : camera_x + 1;
    }
    if (glfwGetKey(frame, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        camera_x = (camera_x - 1) < 0 ? 0 : camera_x - 1;
    }
    if (glfwGetKey(frame, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        camera_z++;
    }
    if (glfwGetKey(frame, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        camera_z--;
    }
    if (glfwGetKey(frame, GLFW_KEY_W) == GLFW_PRESS)
    {
        speed = 0.1;
    }
    if (glfwGetKey(frame, GLFW_KEY_A) == GLFW_PRESS)
    {
        speed = 0.1;
        angle -= M_PI * 0.5;
    }
    if (glfwGetKey(frame, GLFW_KEY_S) == GLFW_PRESS)
    {
        speed = -0.1;
    }
    if (glfwGetKey(frame, GLFW_KEY_D) == GLFW_PRESS)
    {
        speed = 0.1;
        angle += M_PI * 0.5;
    }

    double mouseX, mouseY;
    glfwGetCursorPos(frame, &mouseX, &mouseY);

    int deltaX = (int)mouseX - centerX;
    int deltaY = (int)mouseY - centerY;
        
    if (deltaX != 0 || deltaY != 0)
    {
        camera_z -= deltaX * mouseSensitivity;
        camera_x -= deltaY * mouseSensitivity;
            
        if (camera_x > 180.0f) camera_x = 180.0f;
        if (camera_x < 0.0f) camera_x = 0.0f;
            
        glfwSetCursorPos(frame, centerX, centerY);
    }

    if (speed != 0)
    {
        x += sinf(angle) * speed;
        y += cosf(angle) * speed;
    }
}

/* Load title from info.txt file */
void loadGameTitle() 
{
    /* If you change this, you will be fired */

    FILE* file = fopen("./info.txt", "r");
    if (!file) 
    {
        printf("Could not open info.txt!\n");
        
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