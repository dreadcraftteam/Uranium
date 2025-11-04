// AABB collision and basic physics 
// By DREADCRAFT
//

#ifndef COLLISION_H
#define COLLISION_H

#include "base.h"

#include "defines.h"
#include "../engine/umap.h"

/* Collision constants  */
extern const float playerFeetOffset;
extern const float skinWidth;
extern bool physicsEnabled;

/* Wireframe stuff */ 
extern int collisionWireframe;

extern float wireframeColorRed;
extern float wireframeColorGreen;
extern float wireframeColorBlue;

/* Pushable physics */
extern float pushForce;
extern const float pushableGravity;
extern const float defaultPushableMass;

/* Collision detection functions */
int checkAABBCollision(float pos1[3], float size1[3], float pos2[3], float size2[3]);
void handleCollision(Entity* brush);

/* Player collision functions */
void checkPlayerCollisions(Entity* map);
void checkGrounded(Entity* map);
void validateSpawnPosition(Entity* map);

/* Pushable object functions */
void handlePushableCollision(Entity* pushable);
void updatePushables(Entity* map);
bool isPlayerPushing(Entity* pushable);

/* Drawing AABB collision wireframes */
void drawPlayerWireframeAABB(void);
void drawEntityWireframeAABB(Entity* entity);

#endif // COLLISION_H
