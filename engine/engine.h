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
int engine_main();

/* This is very basic and low-level input */
void basicInputHandle(GLFWwindow* frame);

/* Load title from info.txt file */
void loadGameTitle();

#endif // ENGINE_H
