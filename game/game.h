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

/* Updating */
void gameUpdate(GLFWwindow* frame);

/* Shutdown everything*/
void gameShutdown();

#endif // GAME_H
