// Loading entites from game on engine
// By DREADCRAFT
//

#ifndef GAME_UMAP_H
#define GAME_UMAP_H

#include "base.h"

#include "entities/brush.h"
#include "entities/light.h"
#include "entities/pushable.h"
#include "../engine/umap.h"

/* Entity type parsing */
EntityType gameParseEntityType(const char* str);

/* Entity data parsing */
void gameParseEntityData(Entity* entity, const char* key, const char* val);

/* Rendering the map */
void gameRenderMap(Entity* head, float camPos[3]);

/* Setup lights */
void gameSetupLights(Entity* head);

/* Map load */
void gameMapLoad(const char* filename);

#endif // GAME_UMAP_H
