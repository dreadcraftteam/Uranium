// This is the main file in the engine project
// By DREADCRAFT
//

#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"
#include "dlfcn.h"
#include "math.h"
#include "string.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_messagebox.h"

#include "GL/glew.h"
#include "GL/gl.h"

#include "engine_variables.h"

/* Main method for engine project */
int engine_main(int argc, char* argv[])
{
    /* Read title from info.txt file */
    read_title();

    /* Some stuff */
    void *load_handle;
    char *load_error;

    void (*game_init)();
    void (*game_render)();
    void (*game_shutdown)();

    /* Initialize SDL */
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        return 1;
    }

    /* Set OpenGL attributes before creating the window */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    /* Creating window */
    SDL_Window* frame = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (!frame)
    {
        SDL_Quit();

        return 1;
    }

    /* Create OpenGL context */
    SDL_GLContext glContext = SDL_GL_CreateContext(frame);
    if (!glContext)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", "OpenGL context creation failed!", NULL);

        printf("OpenGL context creation failed!\n");
        SDL_DestroyWindow(frame);
        SDL_Quit();

        return 1;
    }

    /* Initialize GLEW */
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();

    if (glewError != GLEW_OK)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", "GLEW initialization failed!", NULL);

        printf("GLEW initialization failed!\n");
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(frame);
        SDL_Quit();

        return 1;
    }

    /* Open game.so file */
    load_handle = dlopen("./bin/game.so", RTLD_LAZY);
    
    game_init = dlsym(load_handle, "game_init");
    game_render = dlsym(load_handle, "game_render");
    game_shutdown = dlsym(load_handle, "game_shutdown");

    if (!load_handle)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", "Failed to load game.so!", NULL);

        printf("Failed to load game.so!\n");

        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(frame);
        SDL_Quit();


        return 1;
    }

    if ((load_error = dlerror()) != NULL)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", "Failed to load game.so functions!", NULL);

        printf("Failed to load game.so functions!\n");

        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(frame);
        SDL_Quit();

        return 1;
    }

    /* Game initialization */
    game_init();

    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }

            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {
                    running = false;
                }
            }

            handleMouse(&event);
        }

        /* Configuring OpenGL */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(gl_red, gl_green, gl_blue, gl_alpha);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        float aspect = (float)width / (float)height;
        glFrustum(-aspect * 0.1f, aspect * 0.1f, -0.1f, 0.1f, 0.1f, 100.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glEnable(GL_DEPTH_TEST);
        glTranslatef(0.0f, 0.0f, zoom);

        glRotatef(rot_x, 1.0f, 0.0f, 0.0f);
        glRotatef(rot_y, 0.0f, 1.0f, 0.0f);

        /* Game rendering */
        game_render();
        
        SDL_GL_SwapWindow(frame);
    }

    /* Game shutdown */
    game_shutdown();

    /* Remove from game.so from RAM */
    dlclose(load_handle);

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(frame);
    SDL_Quit();

    return 0;
}

/* Mouse control */
void handleMouse(SDL_Event* event, SDL_Window* frame)
{
    switch(event->type)
    {
        case SDL_MOUSEBUTTONDOWN:
            if(event->button.button == SDL_BUTTON_LEFT)
	        {
                mouse_down = true;
                last_x = event->button.x;
                last_y = event->button.y;

		        /* Hide cursor when clicking on cube */
                SDL_ShowCursor(SDL_DISABLE);
                cursor_hidden = true;

                /* Capture mouse for unlimited movement */
                SDL_CaptureMouse(SDL_TRUE);
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if(event->button.button == SDL_BUTTON_LEFT)
            {
                mouse_down = false;

		        /* Show cursor when releasing button */
                SDL_ShowCursor(SDL_ENABLE);
                cursor_hidden = false;

                /* Release mouse capture */
                SDL_CaptureMouse(SDL_FALSE);
            }
            break;

        case SDL_MOUSEMOTION:
            if(mouse_down)
	        {
                int dx = event->motion.x - last_x;
                int dy = event->motion.y - last_y;

                rot_y += dx * 0.5f;
                rot_x += dy * 0.5f;

                last_x = event->motion.x;
                last_y = event->motion.y;

                /* Warp mouse to center for unlimited rotation */
                if(cursor_hidden)
                {
                    SDL_WarpMouseInWindow(frame, width/2, height/2);

                    last_x = width/2;
                    last_y = height/2;
                }
            }
            break;

        case SDL_MOUSEWHEEL:
            /* Zoom in/out with mouse wheel */
            zoom += event->wheel.y * 0.5f;
            /* Limit zoom range */
            if(zoom > -1.0f) zoom = -1.0f;
            if(zoom < -10.0f) zoom = -10.0f;
            break;
    }
}

/* Read title from info.txt file */
void read_title() 
{
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