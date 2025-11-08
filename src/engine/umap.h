// Map system for Uranium (engine)
// By DREADCRAFT
//

#ifndef UMAP_H
#define UMAP_H

#include "base.h"
#include "GLFW/glfw3.h"
#include "variables.h"

#include "../game/entities/brush.h"
#include "../game/entities/pushable.h"
#include "../game/entities/light.h"

/* Entity types */
typedef enum 
{
    ENTITY_BRUSH,
    ENTITY_LIGHT,
    ENTITY_PUSHABLE,
    ENTITY_UNKNOWN
} EntityType;

/* Spawn point */
typedef struct 
{
    float position[3];
} SpawnPoint;

/* Entity structure */
typedef struct Entity 
{
    EntityType type;
    
    union 
    {
        Brush      brush;
        Light      light;
        SpawnPoint spawn;
        Pushable   pushable;
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

/* Setup lights */
void setupLights(Entity* head);

/* Parse entity data */
void parseEntityData(Entity* entity, const char* key, const char* val);

/* Parse entity type */
EntityType parseEntityType(const char* str);

extern Entity* mapLoad;

#endif // UMAP_H
