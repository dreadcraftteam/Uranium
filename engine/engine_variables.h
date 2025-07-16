// Variables and Other low-level Stuff
// By DREADCRAFT
// 

#ifndef ENGINE_VARIABLES_H
#define ENGINE_VARIABLES_H

#include "base.h"

/* Some defines */
#define M_PI 3.14159265358979323846

/* General variables for engine */
static int width = 800;
static int height = 600;

static char* title;

/* Default clear color for OpenGL */
static float glRed = 0.0f / 255.0f;
static float glGreen = 0.0f / 255.0f;
static float glBlue = 0.0f / 255.0f;
static float glAlpha = 0.0f / 255.0f;

/* Mouse control variables */
static int lastX = 0;
static int lastY = 0;
static int mouseX;
static int mouseY;

static bool mouseDown = false;
static bool cursorHidden = false;

#endif // ENGINE_VARIABLES_H
