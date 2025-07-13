// Technically it's a PlayerController (Why this in Engine??)
// By DREADCRAFT
//

#include "stdio.h"
#include "stdbool.h"

#include "GL/glew.h"
#include "GL/gl.h"

#include "engine_variables.h"
#include "engine_camera.h"

float camera_x = 85.0f;
float camera_z = 0.0f;

float x = 0;
float y = 0;
float z = 0;

/* Technically it's a PlayerController */
void camera()
{
    /* Camera position */
    glRotatef(-camera_x, 1, 0, 0);
    glRotatef(-camera_z, 0, 0, 1);

    glTranslatef(-x, -y, -3.0f);
}
