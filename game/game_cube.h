// Header for cube Model
// By DREADCRAFT
//

#include "stdio.h"
#include "stdbool.h"

#include "GL/glew.h"
#include "GL/gl.h"

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

/* Some stuff */
void drawCube();