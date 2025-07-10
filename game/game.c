// This is the main file in the game project
// By DREADCRAFT
// 

#include "stdio.h"
#include "stdbool.h"

#include "GL/glew.h"
#include "GL/gl.h"

/* Rotation cube Variables */
float rot_x = 0.0f;
float rot_y = 0.0f;
float zoom = -3.5f;
float cube_sizeX = 1.0f;
float cube_sizeY = 1.0f; 
float cube_sizeZ = 1.0f;

bool mouse_down = false;
bool cursor_hidden = false;

int last_x = 0; 
int last_y = 0;

/* Main method for engine project */
int game_main()
{
    drawCube();

    return 0;
}

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
