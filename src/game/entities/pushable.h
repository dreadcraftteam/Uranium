// Pushable entity for Uranium (engine)
// By DREADCRAFT
//

#ifndef PUSHABLE_H
#define PUSHABLE_H

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
    float mass;
} Pushable;

/* Initialization */
void pushableInit(Pushable* pushable);

/* Drawing */
void pushableDrawing(Pushable* pushable);

/* Parsing entity data */
void pushableParseData(Pushable* pushable, const char* key, const char* val);

#endif // PUSHABLE_H
