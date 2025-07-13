// Technically it's a PlayerController (Why this in Engine??)
// By DREADCRAFT
//

#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include "stdio.h"

#include "GL/glew.h"
#include "GL/gl.h"

#include "engine_variables.h"

extern float camera_x;
extern float camera_z;

extern float x;
extern float y;
extern float z;

void camera();

#endif // ENGINE_CAMERA_H
