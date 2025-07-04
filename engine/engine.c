// This is the main file in the engine project
// By DREADCRAFT, June 2025
//

#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"
#include "dlfcn.h"
#include "math.h"

#include "SDL2/SDL.h"

#include "GL/glew.h"
#include "GL/gl.h"

#include "engine_variables.h"

/* Main method for engine project */
int engine_main(int argc, char* argv[])
{
    /* Call function for game load */
    load_game();
    
    /* Window on SDL (Simple DirectMedia Layer) */
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        return 1;
    }

    /* Set OpenGL attributes before creating the window */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

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
        fprintf(stderr, "OpenGL context creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(frame);
        SDL_Quit();
        return 1;
    }

    /* Initialize GLEW */
    glewExperimental = GL_TRUE;

    GLenum glewError = glewInit();

    if (glewError != GLEW_OK)
    {
        fprintf(stderr, "GLEW initialization failed: %s\n", glewGetErrorString(glewError));
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(frame);
        SDL_Quit();
        return 1;
    }

    /* Enable VSYNC */
    SDL_GL_SetSwapInterval(1);

    /* Enable DEPTH testing */
    glEnable(GL_DEPTH_TEST);

    SDL_Event event;
    
    /* Cycle */
    while (running)
    {
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            
            /* Close the window */
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {
                    running = false;
                }
            }

	    /* Handle mouse events */
            handleMouse(&event);
        }

  	/* Rotation Cube Code */

 	/* Clear buffers */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(gl_red, gl_green, gl_blue, gl_alpha);

        /* Setup projection matrix */
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float aspect = (float)width / (float)height;
        glFrustum(-aspect * 0.1f, aspect * 0.1f, -0.1f, 0.1f, 0.1f, 100.0f);

        /* Setup modelview matrix */
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, zoom);
        
        /* Apply rotations */
        glRotatef(rotX, 1.0f, 0.0f, 0.0f);
        glRotatef(rotY, 0.0f, 1.0f, 0.0f);

        /* Draw cube */
        drawCube();

        /* Swap buffers */
        SDL_GL_SwapWindow(frame);
    }
    
    SDL_GL_DeleteContext(glContext);

    SDL_DestroyWindow(frame);
    SDL_Quit();

    return 0;
}

/* Load a file game.so to engine project */
void load_game()
{
    void *load_handle;
    char *load_error;

    void (*game_main)();

    /* Open game file */
    load_handle = dlopen("./bin/game.so", RTLD_LAZY);

    if (!load_handle) 
    {
        printf("Failed to load game.so!\n");

        return 1;
    }
    else
    {
        printf("Game.so Loaded!\n");
    }

    /* Load main function */
    game_main = dlsym(load_handle, "game_main");

    if ((load_error = dlerror()) != NULL)
    {
        printf("Failed to load game function!\n");
  
        return 1;
    }
    
    /* Call main function */
    game_main();

    dlclose(load_handle);
}

/* Do I even need to explain what this is? */
void drawCube() 
{
    glBegin(GL_QUADS);

        /* Front face (red) */
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-cubeSizeX, -cubeSizeY,  cubeSizeZ);
        glVertex3f( cubeSizeX, -cubeSizeY,  cubeSizeZ);
        glVertex3f( cubeSizeX,  cubeSizeY,  cubeSizeZ);
        glVertex3f(-cubeSizeX,  cubeSizeY,  cubeSizeZ);
        
        /* Back face (green) */
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-cubeSizeX, -cubeSizeY, -cubeSizeZ);
        glVertex3f(-cubeSizeX,  cubeSizeY, -cubeSizeZ);
        glVertex3f( cubeSizeX,  cubeSizeY, -cubeSizeZ);
        glVertex3f( cubeSizeX, -cubeSizeY, -cubeSizeZ);
        
        /* Top face (blue) */
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-cubeSizeX,  cubeSizeY, -cubeSizeZ);
        glVertex3f(-cubeSizeX,  cubeSizeY,  cubeSizeZ);
        glVertex3f( cubeSizeX,  cubeSizeY,  cubeSizeZ);
        glVertex3f( cubeSizeX,  cubeSizeY, -cubeSizeZ);
        
        /* Bottom face (yellow) */
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex3f(-cubeSizeX, -cubeSizeY, -cubeSizeZ);
        glVertex3f( cubeSizeX, -cubeSizeY, -cubeSizeZ);
        glVertex3f( cubeSizeX, -cubeSizeY,  cubeSizeZ);
        glVertex3f(-cubeSizeX, -cubeSizeY,  cubeSizeZ);
        
        /* Right face (magenta) */
        glColor3f(1.0f, 0.0f, 1.0f);
        glVertex3f( cubeSizeX, -cubeSizeY, -cubeSizeZ);
        glVertex3f( cubeSizeX,  cubeSizeY, -cubeSizeZ);
        glVertex3f( cubeSizeX,  cubeSizeY,  cubeSizeZ);
        glVertex3f( cubeSizeX, -cubeSizeY,  cubeSizeZ);

        /* Left face (cyan) */
        glColor3f(0.0f, 1.0f, 1.0f);
        glVertex3f(-cubeSizeX, -cubeSizeY, -cubeSizeZ);
        glVertex3f(-cubeSizeX, -cubeSizeY,  cubeSizeZ);
        glVertex3f(-cubeSizeX,  cubeSizeY,  cubeSizeZ);
        glVertex3f(-cubeSizeX,  cubeSizeY, -cubeSizeZ);
   
    glEnd();
}

/* Mouse control */
void handleMouse(SDL_Event* event, SDL_Window* frame)
{
    switch(event->type) 
    {
        case SDL_MOUSEBUTTONDOWN:
            if(event->button.button == SDL_BUTTON_LEFT) 
	    {
                mouseDown = true;
                lastX = event->button.x;
                lastY = event->button.y;
           	
		/* Hide cursor when clicking on cube */
                SDL_ShowCursor(SDL_DISABLE);
                cursorHidden = true;
                
                /* Capture mouse for unlimited movement */
                SDL_CaptureMouse(SDL_TRUE);
            }
            break;
            
        case SDL_MOUSEBUTTONUP:
            if(event->button.button == SDL_BUTTON_LEFT) 
            {
                mouseDown = false;

		/* Show cursor when releasing button */
                SDL_ShowCursor(SDL_ENABLE);
                cursorHidden = false;
                
                /* Release mouse capture */
                SDL_CaptureMouse(SDL_FALSE);
            }
            break;

        case SDL_MOUSEMOTION:
            if(mouseDown) 
	    {
                int dx = event->motion.x - lastX;
                int dy = event->motion.y - lastY;
                
                rotY += dx * 0.5f;
                rotX += dy * 0.5f;
                
                lastX = event->motion.x;
                lastY = event->motion.y;

                /* Warp mouse to center for unlimited rotation */
                if(cursorHidden) {
                    SDL_WarpMouseInWindow(frame, width/2, height/2);
                    lastX = width/2;
                    lastY = height/2;
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
