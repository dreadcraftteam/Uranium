// An example of a pyramid model
// By DREADCRAFT
//

#include "stdio.h"
#include "stdbool.h"

#include "GL/glew.h"
#include "GL/gl.h"

#include "func_pyramid.h"

/* Drawing pyramid */
void drawPyramid()
{
    /* Pyramid Position */
    glTranslatef(0.0f, 0.0f, 1.0f);

    glBegin(GL_TRIANGLES);
    
        /* Front face (red) */
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, pyramid_sizeY, 0.0f); 
        glVertex3f(-pyramid_sizeX, -pyramid_sizeY, pyramid_sizeZ);
        glVertex3f(pyramid_sizeX, -pyramid_sizeY, pyramid_sizeZ);
        
        /* Right face (magenta) */
        glColor3f(1.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, pyramid_sizeY, 0.0f);      
        glVertex3f(pyramid_sizeX, -pyramid_sizeY, pyramid_sizeZ);
        glVertex3f(pyramid_sizeX, -pyramid_sizeY, -pyramid_sizeZ);
        
        /* Back face (green) */
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, pyramid_sizeY, 0.0f); 
        glVertex3f(pyramid_sizeX, -pyramid_sizeY, -pyramid_sizeZ);
        glVertex3f(-pyramid_sizeX, -pyramid_sizeY, -pyramid_sizeZ);
        
        /* Left face (cyan) */
        glColor3f(0.0f, 1.0f, 1.0f);
        glVertex3f(0.0f, pyramid_sizeY, 0.0f);
        glVertex3f(-pyramid_sizeX, -pyramid_sizeY, -pyramid_sizeZ);
        glVertex3f(-pyramid_sizeX, -pyramid_sizeY, pyramid_sizeZ);
   
    glEnd();
    
    glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex3f(-pyramid_sizeX, -pyramid_sizeY, -pyramid_sizeZ);
        glVertex3f(pyramid_sizeX, -pyramid_sizeY, -pyramid_sizeZ);
        glVertex3f(pyramid_sizeX, -pyramid_sizeY, pyramid_sizeZ);
        glVertex3f(-pyramid_sizeX, -pyramid_sizeY, pyramid_sizeZ);
    glEnd();
}
