// Map system for Uranium (engine)
// By DREADCRAFT
//

#include "base.h"

#include "GLFW/glfw3.h"
#include "stb/stb_image.h"

#include "../materialsystem/materialsystem.h"
#include "umap.h"
#include "variables.h"

#include "../game/entities/brush.h"
#include "../game/entities/pushable.h"
#include "../game/entities/light.h"
#include "../game/umap_game.h"

Entity* mapLoad = NULL;

/* Load error material if material not found */ 
static GLuint loadErrorTexture()
{
    Material* errorMat = loadMaterial("engine/error.umf");
    
    if(errorMat && errorMat->texture)
    {
        return errorMat->texture->textureId;
    }
    
    return 0;
}

/* Load the texture */
static GLuint loadTexture(const char* filename)
{
    Material* mat = loadMaterial(filename);

    if(mat && mat->texture)
    {
        return mat->texture->textureId;
    }

    static GLuint errorTexture = 0;
    if(errorTexture == 0)
    {
        errorTexture = loadErrorTexture();
    }
    
    return errorTexture;
}

/* Counting */
int countBrushes(Entity* head)
{
    int count = 0;

    for (Entity* e = head; e != NULL; e = e->next)
        if (e->type == ENTITY_BRUSH) count++;

    return count;
}

/* Getting the brush array */
Entity** getBrushArray(Entity* head)
{
    int count = countBrushes(head);
    Entity** arr = malloc(sizeof(Entity*) * count);
    int i = 0;

    for (Entity* e = head; e != NULL; e = e->next)
        if (e->type == ENTITY_BRUSH) arr[i++] = e;

    return arr;
}

/* Parse entity type */
EntityType parseEntityType(const char* str)
{
    return gameParseEntityType(str);
}

/* Parse entity data */
void parseEntityData(Entity* entity, const char* key, const char* val)
{
    gameParseEntityData(entity, key, val);
}

/* Rendering the map */
void renderMap(Entity* head, float camPos[3])
{
    gameRenderMap(head, camPos);
}

/* Setup lights */
void setupLights(Entity* head)
{
    gameSetupLights(head);
}

/* Load the map */
Entity* loadMap(const char* filename)
{
    FILE* f = fopen(filename, "r");

    if (!f)
    {
        Error("Failed to open map file: %s\n", filename);
        return NULL;
    }

    gameMapLoad(filename);
    Entity* head = NULL;
    Entity* tail = NULL;
    Entity* current = NULL;

    char line[256];

    while (fgets(line, sizeof(line), f))
    {
        line[strcspn(line, "\r\n")] = 0;

        if (strcmp(line, "[entity]") == 0)
        {
            current = calloc(1, sizeof(Entity));
            if (!current)
            {
                Error("Failed to allocate memory for entity\n");
                fclose(f);

                return NULL;
            }
            
            current->type = ENTITY_UNKNOWN;
            current->next = NULL;

            if (!head) 
            {
                head = current;
                tail = current;
            }
            else 
            {
                tail->next = current;
                tail = current;
            }
        } 
        else if (current && strlen(line) > 0)
        {
            char key[64], val[128];
            if (sscanf(line, "%63[^=]=%127[^\n]", key, val) == 2)
            {
                if (strcmp(key, "type") == 0)
                {
                    current->type = parseEntityType(val);
                }
                else
                {
                    parseEntityData(current, key, val);
                }
            }
        }
    }

    fclose(f);

    // Reset camera position
    cameraX = 85.0f;
    cameraY = 0.0f;
    cameraZ = 0.0f;

    return head;
}

void freeMap(Entity* head)
{
    Entity* e = head;

    while (e)
    {
        Entity* next = e->next;
        free(e);
        e = next;
    }
}
