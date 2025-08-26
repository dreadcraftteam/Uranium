// GameUI pause menu
// By DREADCRAFT
//

#ifndef PAUSE_H
#define PAUSE_H

#include "base.h"

#include "GLFW/glfw3.h"

#include "../variables.h"

void renderPauseMenu(GLFWwindow* window);
void updatePauseMenu(GLFWwindow* window);
void togglePause();
bool isGamePaused();

#endif // PAUSE_H