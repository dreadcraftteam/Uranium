// Draws a platform on which you can walk
// By DREADCRAFT
//

#include "stdio.h"
#include "math.h"

#include "GL/glew.h"
#include "GL/gl.h"

#include "engine_ground.h"

float vertices[] = {1,1,0, 1,-1,0, -1,-1,0, -1,1,0};

void drawGround()
{
    glEnableClientState(GL_VERTEX_ARRAY);

        glVertexPointer(3, GL_FLOAT, 0, &vertices);
        for(int i = -5; i < 5; i++)
        {
            for(int j = -5; j < 5; j++)
            {
                glPushMatrix();

                    if ((i+j) % 2 == 0) glColor3f(0.0f, 0.4f, 0.0f);
                    else glColor3f(1.0f, 1.0f, 1.0f);
                    glTranslatef(i*2, j*2, 0);
                    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

                glPopMatrix();
            }
        }

    glDisableClientState(GL_VERTEX_ARRAY);
}
