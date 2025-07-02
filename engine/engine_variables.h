// Variables and Other low-level Stuff
// By DREADCRAFT, June 2025
// 

#ifndef ENGINE_VARIABLES_H
#define ENGINE_VARIABLES_H

#include <stdio.h>
#include <stdbool.h>

/* General variables for engine */
int width = 800;
int height = 600;

char* title = "Uranium";
char* version = "v1.0.0";

bool running = true;

/* Default OpenGL clear Color */
float gl_red = 32.0f / 255.0f;
float gl_green = 32.0f / 255.0f;
float gl_blue = 32.0f / 255.0f;
float gl_alpha = 255.0f / 1.0f;

#endif
