// This is the main file in the game project
// By DREADCRAFT
//

#ifndef GAME_H
#define GAME_h

#include "base.h"

#include "GLFW/glfw3.h"

#include "player.h"

/* Write here everything that needs to be initialized once */
void game_init();

/* Updating */
void game_update(GLFWwindow* frame);

/* Shutdown everything*/
void game_shutdown();

#endif // GAME_H
