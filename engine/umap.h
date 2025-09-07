// Map system, thanks to ThePixelMoon for providing the base code
// By DREADCRAFT
//

#ifndef UMAP_H
#define UMAP_H

#include "base.h"

#include "GLFW/glfw3.h"

#include "variables.h"

/* Some structs*/
typedef enum 
{
    ENTITY_BRUSH,
    ENTITY_LIGHT,
    ENTITY_UNKNOWN
} EntityType;

typedef struct 
{
    float position[3];
    float size[3];
    float color[3];
    GLuint textureId;
} Brush;

typedef struct 
{
    float position[3];
    float radius;
} Light;

typedef struct 
{
    float position[3];
} SpawnPoint;

typedef struct Entity 
{
    EntityType type;
    
    union 
    {
        Brush      brush;
        Light      light;
        SpawnPoint spawn;
    };
    struct Entity* next;
} Entity;

/* Getting the brush array */
Entity** getBrushArray(Entity* head);

/* Counting brushes */
int countBrushes(Entity* head);

/* Load the map*/
Entity* loadMap(const char* filename);

/* Removing map from memory */
void freeMap(Entity* head);

/* Rendering */
void renderMap(Entity* head, float camPos[3]);

/* Initializing lights */
void setupLights(Entity* head);

extern Entity* mapLoad;

#endif // UMAP_H

