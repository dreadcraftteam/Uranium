// Basic variables and Other low-level stuff for engine
// By DREADCRAFT
// 

#ifndef VARIABLES_H
#define VARIABLES_H

#include "base.h"

#include "buildnumber.h"
#include "defines.h"

/* General variables for engine */ 
extern int screenWidth; 
extern int screenHeight; 

extern char* title; 

static char* version = "1.0.0";

static char* protocol = "1"; 

extern int build_number;

extern bool running; 
extern bool focused;

/* Player variables that are required to be in the engine */ 
extern float playerX; 
extern float playerY; 
extern float playerZ; 

extern float cameraX; 
extern float cameraY; 
extern float cameraZ; 

extern double mouseX; 
extern double mouseY; 

extern bool enableNoClip;

/* Debug panel stuff */
extern int enableDebugPanel;
extern int enableFPSCounter;

#endif // VARIABLES_H 
