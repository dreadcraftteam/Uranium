// This is the main file in the game project
// By DREADCRAFT
//

#include "base.h"

#include "GLFW/glfw3.h"

#include "player.h"
#include "game.h"

/* Write here everything that needs to be initialized once */
void gameInit()
{
    playerInit();
}

/* Rendering the game */
void gameRender(GLFWwindow* frame)
{
    /* The player should always be created first! */
    player();

    /* Camera input */
    handleMovementInput(frame);
    handleMouseInput(frame);
}

/* Updating */
void gameUpdate()
{
    
}

/* Shutdown everything*/
void gameShutdown()
{
    
}

