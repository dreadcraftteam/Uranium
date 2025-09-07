// Basic variables and Other low-level stuff for engine
// By DREADCRAFT
// 

#ifndef VARIABLES_H
#define VARIABLES_H

#include "base.h"

/* General variables for engine */
static int width = 800;
static int height = 600;

static char* title;

extern bool running;

/* Default clear color for OpenGL */
static float glRed = 0.0f / 255.0f;
static float glGreen = 0.0f / 255.0f;
static float glBlue = 0.0f / 255.0f;
static float glAlpha = 0.0f / 255.0f;

/* Render mode Stuff */
extern int renderMode;

static float wireframeRed = 255.0f;
static float wireframeGreen = 255.0f;
static float wireframeBlue = 255.0f;

#endif // VARIABLES_H

