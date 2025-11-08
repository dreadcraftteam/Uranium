// Light entity for Uranium (engine)
// By DREADCRAFT
//

#ifndef LIGHT_H
#define LIGHT_H

#include "base.h"
#include "GLFW/glfw3.h"

// Forward declaration to avoid circular dependency
typedef struct Entity Entity;

typedef struct 
{
    float position[3];
    float radius;
    float color[3];
} Light;

/* Initialization */
void lightInit(Light* light);

/* Setup lights */
void lightSetup(Entity* head);

/* Parsing entity data */
void lightParseData(Light* light, const char* key, const char* val);

#endif // LIGHT_H
