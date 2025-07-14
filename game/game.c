// This is the main file in the game project
// By DREADCRAFT
//

#include "stdio.h"
#include "stdbool.h"

#include "GL/glew.h"
#include "GL/gl.h"

#include "GLFW/glfw3.h"

#include "examples/func_pyramid.h"
#include "examples/func_cube.h"
#include "func_ground.h"

/* Write here everything that needs to be initialized once */
void game_init()
{
    glewInit();
    
    if (glewInit() != GLEW_OK) 
    {
        printf("OpenGL initialization failed!\n");
        
        return -1;
    }
}

/* Rendering */
void game_render()
{
    drawGround();
}

/* Shutdown everything*/
void game_shutdown()
{

}