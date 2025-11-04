// This is the main file in the engine project
// By DREADCRAFT
//

#ifndef ENGINE_H
#define ENGINE_H

#include "base.h"

#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

#include "audio/audio.h"
#include "defines.h"
#include "dynlib.h"
#include "engine.h"
#include "umap.h"
#include "variables.h"

/* Main method for engine project */
int engineMain(int argc, char* argv[]);

/* This is very basic and low-level input */
void baseInputHandle(GLFWwindow* frame);

/* Load title and version from info.txt file */
void loadGameInfo(void);

/* Begin game rendering and configuring OpenGL */
void beginGameRendering(void);

/* End game rendering */
void endGameRendering(void);

/* Window resize callback */
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

/* Set window size */
void setWindowSize(int width, int height);

#endif // ENGINE_H
