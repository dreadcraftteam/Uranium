// This is the main file in the game project
// By DREADCRAFT
//

#ifndef GAME_H
#define GAME_h

#include "base.h"

#include "GLFW/glfw3.h"

#include "player.h"

/* Write here everything that needs to be initialized once */
void gameInit(void);

/* Rendering the game */
void gameRender(void);

/* Updating */
void gameUpdate(GLFWwindow* window);

/* Input handle for Game */
void gameInputHandle(GLFWwindow* frame);

/* Shutdown everything*/
void gameShutdown(void);

#endif // GAME_H
