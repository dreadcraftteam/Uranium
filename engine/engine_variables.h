// Variables and Other low-level Stuff
// By DREADCRAFT
// 

#ifndef ENGINE_VARIABLES_H
#define ENGINE_VARIABLES_H

#include "stdio.h"
#include "stdbool.h"

/* General variables for engine */
const static int width = 800;
const static int height = 600;

const static char* title = "Uranium";
const static char* version = "v1.0.0";

static bool running = true;

/* Default clear color for OpenGL */
static float gl_red = 0.0f / 255.0f;
static float gl_green = 0.0f / 255.0f;
static float gl_blue = 0.0f / 255.0f;
static float gl_alpha = 0.0f / 1.0f;

/* Rotation cube Variables */
static float rot_x = 0.0f;
static float rot_y = 0.0f;
static float zoom = -3.5f;
static float cube_sizeX = 1.0f;
static float cube_sizeY = 1.0f; 
static float cube_sizeZ = 1.0f;

static bool mouse_down = false;
static bool cursor_hidden = false;

static int last_x = 0; 
static int last_y = 0;

#endif
