// Brush entity for Uranium (engine)
// By DREADCRAFT
//

#include "base.h"
#include "GLFW/glfw3.h"
#include "brush.h"
#include "../materialsystem/materialsystem.h"

/* Initialization */
void brushInit(Brush* brush)
{
    brush->position[0] = 0.0f;
    brush->position[1] = 0.0f;
    brush->position[2] = 0.0f;
    
    brush->size[0] = 1.0f;
    brush->size[1] = 1.0f;
    brush->size[2] = 1.0f;
    
    brush->color[0] = 1.0f;
    brush->color[1] = 1.0f;
    brush->color[2] = 1.0f;
    
    brush->textureId = 0;
    brush->textureFit = 1;
    brush->ignoreLighting = false;
}

/* Drawing */
void brushDrawing(Brush* brush)
{
    float hx = brush->size[0] / 2.0f;
    float hy = brush->size[1] / 2.0f;
    float hz = brush->size[2] / 2.0f;

    if (brush->ignoreLighting)
    {
        glDisable(GL_LIGHTING);
    }

    if (brush->textureId)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, brush->textureId);

        if (brush->textureFit) 
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        } 
        else 
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
        
        GLfloat mat_diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat mat_ambient[]  = {0.5f, 0.5f, 0.5f, 1.0f};
        GLfloat mat_specular[] = {0.1f, 0.1f, 0.1f, 1.0f};
        GLfloat mat_shininess  = 8.0f;

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mat_specular);
        glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    }
    else
    {
        glDisable(GL_TEXTURE_2D);
            
        GLfloat mat_diffuse[]  = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat mat_ambient[]  = {0.5f, 0.5f, 0.5f, 1.0f};
        GLfloat mat_specular[] = {0.1f, 0.1f, 0.1f, 1.0f};
        GLfloat mat_shininess  = 8.0f;

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mat_specular);
        glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
    }

    float texScaleFrontX = 1.0f, texScaleFrontY = 1.0f;
    float texScaleSideX = 1.0f, texScaleSideY = 1.0f;
    float texScaleTopX = 1.0f, texScaleTopY = 1.0f;

    if (!brush->textureFit) 
    {
        texScaleFrontX = brush->size[0] / 2; 
        texScaleFrontY = brush->size[1] / 2;
        
        texScaleSideX = brush->size[2] / 2;
        texScaleSideY = brush->size[1] / 2;
        
        texScaleTopX = brush->size[0] / 2;
        texScaleTopY = brush->size[2] / 2;
    }

    glBegin(GL_QUADS);

        glNormal3f(0, 0, 1);
        glTexCoord2f(0, texScaleFrontY); glVertex3f(-hx, -hy, hz);
        glTexCoord2f(texScaleFrontX, texScaleFrontY); glVertex3f(hx, -hy, hz);
        glTexCoord2f(texScaleFrontX, 0); glVertex3f(hx, hy, hz);
        glTexCoord2f(0, 0); glVertex3f(-hx, hy, hz);

        glNormal3f(0, 0, -1);
        glTexCoord2f(0, texScaleFrontY); glVertex3f(hx, -hy, -hz);
        glTexCoord2f(texScaleFrontX, texScaleFrontY); glVertex3f(-hx, -hy, -hz);
        glTexCoord2f(texScaleFrontX, 0); glVertex3f(-hx, hy, -hz);
        glTexCoord2f(0, 0); glVertex3f(hx, hy, -hz);

        glNormal3f(-1, 0, 0);
        glTexCoord2f(0, texScaleSideY); glVertex3f(-hx, -hy, -hz);
        glTexCoord2f(texScaleSideX, texScaleSideY); glVertex3f(-hx, -hy, hz);
        glTexCoord2f(texScaleSideX, 0); glVertex3f(-hx, hy, hz);
        glTexCoord2f(0, 0); glVertex3f(-hx, hy, -hz);

        glNormal3f(1, 0, 0);
        glTexCoord2f(0, texScaleSideY); glVertex3f(hx, -hy, hz);
        glTexCoord2f(texScaleSideX, texScaleSideY); glVertex3f(hx, -hy, -hz);
        glTexCoord2f(texScaleSideX, 0); glVertex3f(hx, hy, -hz);
        glTexCoord2f(0, 0); glVertex3f(hx, hy, hz);

        glNormal3f(0, 1, 0);
        glTexCoord2f(0, texScaleTopY); glVertex3f(-hx, hy, hz);
        glTexCoord2f(texScaleTopX, texScaleTopY); glVertex3f(hx, hy, hz);
        glTexCoord2f(texScaleTopX, 0); glVertex3f(hx, hy, -hz);
        glTexCoord2f(0, 0); glVertex3f(-hx, hy, -hz);

        glNormal3f(0, -1, 0);
        glTexCoord2f(0, texScaleTopY); glVertex3f(-hx, -hy, -hz);
        glTexCoord2f(texScaleTopX, texScaleTopY); glVertex3f(hx, -hy, -hz);
        glTexCoord2f(texScaleTopX, 0); glVertex3f(hx, -hy, hz);
        glTexCoord2f(0, 0); glVertex3f(-hx, -hy, hz);
    
    glEnd();

    if (brush->textureId)
        glDisable(GL_TEXTURE_2D);

    if (brush->ignoreLighting)
    {
        glEnable(GL_LIGHTING);
    }
}

/* Parsing entity data */
void brushParseData(Brush* brush, const char* key, const char* val)
{
    if (strcmp(key, "position") == 0)
    {
        float temp[3];
        sscanf(val, "%f,%f,%f", &temp[0], &temp[1], &temp[2]);
        brush->position[0] = temp[0];
        brush->position[1] = temp[2];
        brush->position[2] = temp[1];
    }
    else if (strcmp(key, "size") == 0)
    {
        float temp[3];
        sscanf(val, "%f,%f,%f", &temp[0], &temp[1], &temp[2]);
        brush->size[0] = temp[0];
        brush->size[1] = temp[2];
        brush->size[2] = temp[1];
    }
    else if (strcmp(key, "color") == 0)
    {
        float temp[3];
        sscanf(val, "%f,%f,%f", &temp[0], &temp[1], &temp[2]);
        brush->color[0] = temp[0];
        brush->color[1] = temp[1];
        brush->color[2] = temp[2];
    }
    else if (strcmp(key, "texture") == 0)
    {
        Material* mat = loadMaterial(val);
        if (mat && mat->texture)
        {
            brush->textureId = mat->texture->textureId;
            
            Material* existingMat = getMaterial(val);
            if (existingMat)
            {
                brush->ignoreLighting = existingMat->ignoreLighting;
            }
        }
    }
    else if (strcmp(key, "textureFit") == 0)
    {
        if (strcmp(val, "yes") == 0) 
        {
            brush->textureFit = 1;
        }
        else if (strcmp(val, "no") == 0) 
        {
            brush->textureFit = 0; 
        }
    }
}
