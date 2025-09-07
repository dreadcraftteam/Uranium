// AABB collision and basic physics 
// By DREADCRAFT
// 

#ifndef COLLISION_H
#define COLLISION_H

#include "base.h"

#include "defines.h"

/* Collision constants */
extern const float playerFeetOffset;
extern const float skinWidth;

/* Collision detection functions */
int checkAABBCollision(float pos1[3], float size1[3], float pos2[3], float size2[3]);
void handleCollision(Entity* brush);

/* Player collision functions */
void checkPlayerCollisions(Entity* map);
void checkGrounded(Entity* map);
void validateSpawnPosition(Entity* map);

#endif // COLLISION_H

