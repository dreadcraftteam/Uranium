// This is the main file in the game project
// By DREADCRAFT
//

#ifndef GAME_H
#define GAME_h

#include "base.h"

#include "GLFW/glfw3.h"

#include "player.h"

/* Write here everything that needs to be initialized once */
void gameInit();

/* Rendering the game */
void gameRender(GLFWwindow* frame);

/* Updating */
void gameUpdate();

/* Shutdown everything*/
void gameShutdown();

#endif // GAME_H

