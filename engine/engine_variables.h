// Variables and Other low-level Stuff
// By DREADCRAFT
// 

#ifndef ENGINE_VARIABLES_H
#define ENGINE_VARIABLES_H

#include "stdio.h"
#include "stdbool.h"
#include "string.h"

/* Some defines */
#define M_PI 3.14159265358979323846

/* General variables for engine */
static int width = 800;
static int height = 600;

static char* title;

static bool running = true;

/* Default clear color for OpenGL */
static float gl_red = 0.0f / 255.0f;
static float gl_green = 0.0f / 255.0f;
static float gl_blue = 0.0f / 255.0f;
static float gl_alpha = 0.0f / 255.0f;

/* Controls and Movement */
static int lastX = 0;
static int lastY = 0;
static int mouseX;
static int mouseY;

static bool mouse_down = false;
static bool cursor_hidden = false;

static float mouseSensitivity = 0.1f;

#endif // ENGINE_VARIABLES_H
