// Variables and Other low-level Stuff
// By DREADCRAFT, June 2025
// 

#ifndef ENGINE_VARIABLES_H
#define ENGINE_VARIABLES_H

#include "stdio.h"
#include "stdbool.h"

/* General variables for engine */
const int width = 800;
const int height = 600;

const char* title = "Uranium";
const char* version = "v1.0.0";

bool running = true;

/* Default OpenGL clear Color */
float gl_red = 0.0f / 255.0f;
float gl_green = 0.0f / 255.0f;
float gl_blue = 0.0f / 255.0f;
float gl_alpha = 0.0f / 1.0f;

/* Rotation cube Variables */
float rot_x = 0.0f;
float rot_y = 0.0f;
float zoom = -3.5f;
float cube_sizeX = 1.0f;
float cube_sizeY = 1.0f; 
float cube_sizeZ = 1.0f;

bool mouse_down = false;
bool cursor_hidden = false;

int last_x = 0; 
int last_y = 0;

#endif
