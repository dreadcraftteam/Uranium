// Loading entites from game on engine
// By DREADCRAFT
//

#include "base.h"

#include "entities/brush.h"
#include "entities/light.h"
#include "entities/pushable.h"
#include "../engine/umap.h"
#include "umap_game.h"

/* Entity type parsing */
EntityType gameParseEntityType(const char* str)
{
    if (strcmp(str, "ent_brush") == 0) return ENTITY_BRUSH;
    if (strcmp(str, "ent_light") == 0) return ENTITY_LIGHT;
    if (strcmp(str, "ent_pushable") == 0) return ENTITY_PUSHABLE;

    return ENTITY_UNKNOWN;
}

/* Entity data parsing */
void gameParseEntityData(Entity* entity, const char* key, const char* val)
{
    switch (entity->type)
    {
        case ENTITY_BRUSH:
            brushParseData(&entity->brush, key, val);
            break;
        case ENTITY_LIGHT:
            lightParseData(&entity->light, key, val);
            break;
        case ENTITY_PUSHABLE:
            pushableParseData(&entity->pushable, key, val);
            break;
        default:
            break;
    }
}

/* Rendering the map */
void gameRenderMap(Entity* head, float camPos[3])
{
    Entity* e = head;
    
    while (e != NULL)
    {
        if (e->type == ENTITY_BRUSH)
        {
            glPushMatrix();
            glTranslatef(e->brush.position[0], e->brush.position[1], e->brush.position[2]);
            brushDrawing(&e->brush);
            glPopMatrix();
        }
        else if (e->type == ENTITY_PUSHABLE)
        {
            glPushMatrix();
            glTranslatef(e->pushable.position[0], e->pushable.position[1], e->pushable.position[2]);
            pushableDrawing(&e->pushable);
            glPopMatrix();
        }
        
        e = e->next;
    }
}

/* Setup lights */
void gameSetupLights(Entity* head)
{
    lightSetup(head);
}

/* Map load */
void gameMapLoad(const char* filename)
{
    FILE* f = fopen(filename, "r");

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
            
            brushInit(&current->brush);
            lightInit(&current->light);
            pushableInit(&current->pushable);

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
    }

    fclose(f);
}
