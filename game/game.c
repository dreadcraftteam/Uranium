// This is the main file in the game project
// By DREADCRAFT
//

#include "base.h"

#include "GLFW/glfw3.h"

#include "player.h"
#include "game.h"
#include "collision.h"
#include "../engine/umap.h"

/* Write here everything that needs to be initialized once */
void gameInit(void)
{
    playerInit();
}

/* Rendering the game */
void gameRender(void)
{
    
}

/* Updating */
void gameUpdate(GLFWwindow* frame)
{
    playerUpdate();

    handleMouseInput(frame);

    if (collisionWireframe == 1) 
    {
        drawPlayerWireframeAABB();

        for (Entity* entity = mapLoad; entity != NULL; entity = entity->next) 
        {
            drawEntityWireframeAABB(entity);
        }
    }
}

/* Input handle for Game */
void gameInputHandle(GLFWwindow* frame)
{
    handleMovementInput(frame);
    handleJumpInput();
    handleDeathInput(frame);
}

/* Shutdown everything*/
void gameShutdown(void)
{
    
}
