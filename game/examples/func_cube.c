// An example of a cube model
// By DREADCRAFT
//

#include "base.h"

#include "func_cube.h"

/* Drawing cube */
void drawCube()
{
    /* Cube Position */
    glTranslatef(0.0f, 0.0f, 1.0f);

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
