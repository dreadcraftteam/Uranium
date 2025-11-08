// Pushable entity for Uranium (engine)
// By DREADCRAFT
//

#include "base.h"
#include "pushable.h"
#include "brush.h"
#include "../materialsystem/materialsystem.h"

/* Initialization */
void pushableInit(Pushable* pushable)
{
    pushable->position[0] = 0.0f;
    pushable->position[1] = 0.0f;
    pushable->position[2] = 0.0f;
    
    pushable->size[0] = 1.0f;
    pushable->size[1] = 1.0f;
    pushable->size[2] = 1.0f;
    
    pushable->color[0] = 1.0f;
    pushable->color[1] = 1.0f;
    pushable->color[2] = 1.0f;
    
    pushable->textureId = 0;
    pushable->textureFit = 1;
    pushable->ignoreLighting = false;
    pushable->mass = 1.0f;
}

/* Drawing */
void pushableDrawing(Pushable* pushable)
{
    // Create a temporary brush structure for drawing
    Brush tempBrush;
    tempBrush.position[0] = pushable->position[0];
    tempBrush.position[1] = pushable->position[1];
    tempBrush.position[2] = pushable->position[2];
    
    tempBrush.size[0] = pushable->size[0];
    tempBrush.size[1] = pushable->size[1];
    tempBrush.size[2] = pushable->size[2];
    
    tempBrush.color[0] = pushable->color[0];
    tempBrush.color[1] = pushable->color[1];
    tempBrush.color[2] = pushable->color[2];
    
    tempBrush.textureId = pushable->textureId;
    tempBrush.textureFit = pushable->textureFit;
    tempBrush.ignoreLighting = pushable->ignoreLighting;
    
    // Use brush drawing function
    brushDrawing(&tempBrush);
}

/* Parsing entity data */
void pushableParseData(Pushable* pushable, const char* key, const char* val)
{
    if (strcmp(key, "position") == 0)
    {
        float temp[3];
        sscanf(val, "%f,%f,%f", &temp[0], &temp[1], &temp[2]);
        pushable->position[0] = temp[0];
        pushable->position[1] = temp[2];
        pushable->position[2] = temp[1];
    }
    else if (strcmp(key, "size") == 0)
    {
        float temp[3];
        sscanf(val, "%f,%f,%f", &temp[0], &temp[1], &temp[2]);
        pushable->size[0] = temp[0];
        pushable->size[1] = temp[2];
        pushable->size[2] = temp[1];
    }
    else if (strcmp(key, "color") == 0)
    {
        float temp[3];
        sscanf(val, "%f,%f,%f", &temp[0], &temp[1], &temp[2]);
        pushable->color[0] = temp[0];
        pushable->color[1] = temp[1];
        pushable->color[2] = temp[2];
    }
    else if (strcmp(key, "texture") == 0)
    {
        Material* mat = loadMaterial(val);
        if (mat && mat->texture)
        {
            pushable->textureId = mat->texture->textureId;
            
            Material* existingMat = getMaterial(val);
            if (existingMat)
            {
                pushable->ignoreLighting = existingMat->ignoreLighting;
            }
        }
    }
    else if (strcmp(key, "textureFit") == 0)
    {
        if (strcmp(val, "yes") == 0) 
        {
            pushable->textureFit = 1;
        }
        else if (strcmp(val, "no") == 0) 
        {
            pushable->textureFit = 0; 
        }
    }
    else if (strcmp(key, "mass") == 0)
    {
        pushable->mass = atof(val);
    }
}
