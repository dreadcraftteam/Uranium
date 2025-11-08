// Brush entity for Uranium (engine)
// By DREADCRAFT
//

#ifndef BRUSH_H
#define BRUSH_H

#include "base.h"
#include "GLFW/glfw3.h"

typedef struct 
{
    float position[3];
    float size[3];
    float color[3];
    GLuint textureId;
    int textureFit;
    bool ignoreLighting;
} Brush;

/* Initialization */
void brushInit(Brush* brush);

/* Drawing */
void brushDrawing(Brush* brush);

/* Parsing entity data */
void brushParseData(Brush* brush, const char* key, const char* val);

#endif // BRUSH_H
