// Cube Model
// By DREADCRAFT
//

#include "stdio.h"
#include "stdbool.h"

#include "GL/glew.h"
#include "GL/gl.h"

#include "game_cube.h"

/* yeah... */
void drawCube()
{
    glBegin(GL_QUADS);

        /* Front face (red) */
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-cube_sizeX, -cube_sizeY,  cube_sizeZ);
        glVertex3f( cube_sizeX, -cube_sizeY,  cube_sizeZ);
        glVertex3f( cube_sizeX,  cube_sizeY,  cube_sizeZ);
        glVertex3f(-cube_sizeX,  cube_sizeY,  cube_sizeZ);
        
        /* Back face (green) */
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-cube_sizeX, -cube_sizeY, -cube_sizeZ);
        glVertex3f(-cube_sizeX,  cube_sizeY, -cube_sizeZ);
        glVertex3f( cube_sizeX,  cube_sizeY, -cube_sizeZ);
        glVertex3f( cube_sizeX, -cube_sizeY, -cube_sizeZ);
        
        /* Top face (blue) */
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(-cube_sizeX,  cube_sizeY, -cube_sizeZ);
        glVertex3f(-cube_sizeX,  cube_sizeY,  cube_sizeZ);
        glVertex3f( cube_sizeX,  cube_sizeY,  cube_sizeZ);
        glVertex3f( cube_sizeX,  cube_sizeY, -cube_sizeZ);
        
        /* Bottom face (yellow) */
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex3f(-cube_sizeX, -cube_sizeY, -cube_sizeZ);
        glVertex3f( cube_sizeX, -cube_sizeY, -cube_sizeZ);
        glVertex3f( cube_sizeX, -cube_sizeY,  cube_sizeZ);
        glVertex3f(-cube_sizeX, -cube_sizeY,  cube_sizeZ);
        
        /* Right face (magenta) */
        glColor3f(1.0f, 0.0f, 1.0f);
        glVertex3f( cube_sizeX, -cube_sizeY, -cube_sizeZ);
        glVertex3f( cube_sizeX,  cube_sizeY, -cube_sizeZ);
        glVertex3f( cube_sizeX,  cube_sizeY,  cube_sizeZ);
        glVertex3f( cube_sizeX, -cube_sizeY,  cube_sizeZ);

        /* Left face (cyan) */
        glColor3f(0.0f, 1.0f, 1.0f);
        glVertex3f(-cube_sizeX, -cube_sizeY, -cube_sizeZ);
        glVertex3f(-cube_sizeX, -cube_sizeY,  cube_sizeZ);
        glVertex3f(-cube_sizeX,  cube_sizeY,  cube_sizeZ);
        glVertex3f(-cube_sizeX,  cube_sizeY, -cube_sizeZ);
   
    glEnd();
}