// AABB collision and basic physics 
// By DREADCRAFT
//

#include "base.h"

#include "defines.h"
#include "player.h"
#include "collision.h"
#include "../engine/umap.h"

/* Collision constants */
const float playerFeetOffset = 5.0f;
const float skinWidth = 0.05f;

bool physicsEnabled = true;

/* Wireframe stuff */
int collisionWireframe = 0;

/* Colors */
float wireframeColorRed = 0.0f / 255.0f;
float wireframeColorGreen = 242.0f / 255.0f;
float wireframeColorBlue = 255.0f / 255.0f;

/* Pushable physics */
float pushForce = 0.1f;
const float pushableGravity = -0.4f;
const float pushableFallThreshold = 0.1f;
const float defaultPushableMass = 1.0f;

/* Check if two AABBs intersect */
int checkAABBCollision(float pos1[3], float size1[3], float pos2[3], float size2[3]) 
{
    return fabs(pos1[0] - pos2[0]) < (size1[0] + size2[0]) &&
           fabs(pos1[1] - pos2[1]) < (size1[1] + size2[1]) &&
           fabs(pos1[2] - pos2[2]) < (size1[2] + size2[2]);
}

/* Handle collision response for a single brush */
void handleCollision(Entity* brush) 
{
    if (enableNoClip) return;

    float playerSize[3] = {playerWidth, playerHeight, playerDepth};
    float brushSize[3] = 
    {
        brush->brush.size[0] / 2.0f,
        brush->brush.size[1] / 2.0f,
        brush->brush.size[2] / 2.0f
    };
    
    float playerPos[3] = {playerX, playerY, playerZ};
    float brushPos[3] = 
    {
        brush->brush.position[0],
        brush->brush.position[1],
        brush->brush.position[2]
    };
    
    float penetrationX = (playerWidth + brushSize[0]) - fabs(playerPos[0] - brushPos[0]);
    float penetrationY = (playerHeight + brushSize[1]) - fabs(playerPos[1] - brushPos[1]);
    float penetrationZ = (playerDepth + brushSize[2]) - fabs(playerPos[2] - brushPos[2]);
    
    // Resolve along the axis with minimum penetration
    if (penetrationX < penetrationY && penetrationX < penetrationZ) 
    {
        if (playerPos[0] < brushPos[0]) 
            playerX -= penetrationX + skinWidth;
        else 
            playerX += penetrationX + skinWidth;
    } 
    else if (penetrationY < penetrationZ) 
    {
        if (playerPos[1] < brushPos[1]) 
            playerY -= penetrationY + skinWidth;
        else 
            playerY += penetrationY + skinWidth;
    } 
    else 
    {
        if (playerPos[2] < brushPos[2]) 
        {
            playerZ -= penetrationZ + skinWidth;
            if (verticalVelocity < 0) 
            {
                isGrounded = true;
                verticalVelocity = 0.0f;
            }
        } 
        else 
        {
            playerZ += penetrationZ + skinWidth;
            if (verticalVelocity > 0) 
            {
                verticalVelocity = 0.0f;
            }
        }
    }
}

/* Handle collision with pushable object */
void handlePushableCollision(Entity* pushable)
{
    if (enableNoClip) return;

    float playerSize[3] = {playerWidth, playerHeight, playerDepth};
    float pushableSize[3] = 
    {
        pushable->pushable.size[0] / 2.0f,
        pushable->pushable.size[1] / 2.0f,
        pushable->pushable.size[2] / 2.0f
    };
    
    float playerPos[3] = {playerX, playerY, playerZ};
    float pushablePos[3] = 
    {
        pushable->pushable.position[0],
        pushable->pushable.position[1],
        pushable->pushable.position[2]
    };
    
    float overlapX = (playerWidth + pushableSize[0]) - fabs(playerPos[0] - pushablePos[0]);
    float overlapY = (playerHeight + pushableSize[1]) - fabs(playerPos[1] - pushablePos[1]);
    float overlapZ = (playerDepth + pushableSize[2]) - fabs(playerPos[2] - pushablePos[2]);
    
    // Always resolve collision first - push player out of the object
    if (overlapX < overlapY && overlapX < overlapZ) 
    {
        // X-axis collision - push player out
        if (playerPos[0] < pushablePos[0]) 
        {
            playerX = pushablePos[0] - pushableSize[0] - playerWidth - skinWidth;
        }
        else 
        {
            playerX = pushablePos[0] + pushableSize[0] + playerWidth + skinWidth;
        }
    } 
    else if (overlapY < overlapZ) 
    {
        // Y-axis collision - push player out
        if (playerPos[1] < pushablePos[1]) 
        {
            playerY = pushablePos[1] - pushableSize[1] - playerDepth - skinWidth;
        }
        else 
        {
            playerY = pushablePos[1] + pushableSize[1] + playerDepth + skinWidth;
        }
    } 
    else 
    {
        // Z-axis collision - push player out
        if (playerPos[2] < pushablePos[2]) 
        {
            playerZ = pushablePos[2] - pushableSize[2] - playerHeight - skinWidth;
            if (verticalVelocity < 0) 
            {
                isGrounded = true;
                verticalVelocity = 0.0f;
            }
        } 
        else 
        {
            playerZ = pushablePos[2] + pushableSize[2] + playerFeetOffset + skinWidth;
            if (verticalVelocity > 0) 
            {
                verticalVelocity = 0.0f;
            }
        }
        
        return; // Don't push object on vertical collisions
    }
    
    float angle = -cameraZ / 180 * PI;
    float moveX = 0.0f, moveY = 0.0f;
    
    if (KEY_DOWN(INPUT_KEY_W)) moveY += 1.0f;
    if (KEY_DOWN(INPUT_KEY_S)) moveY -= 1.0f;
    if (KEY_DOWN(INPUT_KEY_A)) moveX -= 1.0f;
    if (KEY_DOWN(INPUT_KEY_D)) moveX += 1.0f;
    
    if (moveX == 0.0f && moveY == 0.0f) return;
    
    float worldMoveX = sinf(angle) * moveY + cosf(angle) * moveX;
    float worldMoveY = cosf(angle) * moveY - sinf(angle) * moveX;
    
    float newPlayerPos[3] = {playerX, playerY, playerZ};
    float dirToPushableX = pushablePos[0] - newPlayerPos[0];
    float dirToPushableY = pushablePos[1] - newPlayerPos[1];
    
    float dirLength = sqrtf(dirToPushableX * dirToPushableX + dirToPushableY * dirToPushableY);
    if (dirLength > 0) 
    {
        dirToPushableX /= dirLength;
        dirToPushableY /= dirLength;
    }
    
    float dotProduct = worldMoveX * dirToPushableX + worldMoveY * dirToPushableY;
    
    if (dotProduct > 0.5f) 
    {
        float mass = pushable->pushable.mass > 0 ? pushable->pushable.mass : defaultPushableMass;
        float effectivePushForce = pushForce / mass;
        
        pushable->pushable.position[0] += worldMoveX * effectivePushForce;
        pushable->pushable.position[1] += worldMoveY * effectivePushForce;
    }
}

/* Check if player is actively pushing an object */
bool isPlayerPushing(Entity* pushable)
{
    if (enableNoClip) return false;
    
    float playerSize[3] = {playerWidth, playerHeight, playerDepth};
    float pushableSize[3] = 
    {
        pushable->pushable.size[0] / 2.0f,
        pushable->pushable.size[1] / 2.0f,
        pushable->pushable.size[2] / 2.0f
    };
    
    float playerPos[3] = {playerX, playerY, playerZ};
    float pushablePos[3] = 
    {
        pushable->pushable.position[0],
        pushable->pushable.position[1],
        pushable->pushable.position[2]
    };
    
    // Check if player is colliding with pushable
    if (!checkAABBCollision(playerPos, playerSize, pushablePos, pushableSize)) 
    {
        return false;
    }
    
    float angle = -cameraZ / 180 * PI;
    float moveX = 0.0f, moveY = 0.0f;
    
    if (KEY_DOWN(INPUT_KEY_W)) moveY += 1.0f;
    if (KEY_DOWN(INPUT_KEY_S)) moveY -= 1.0f;
    if (KEY_DOWN(INPUT_KEY_A)) moveX -= 1.0f;
    if (KEY_DOWN(INPUT_KEY_D)) moveX += 1.0f;
    
    if (moveX == 0 && moveY == 0) return false;
    
    float worldMoveX = sinf(angle) * moveY + cosf(angle) * moveX;
    float worldMoveY = cosf(angle) * moveY - sinf(angle) * moveX;
    
    float dirX = pushablePos[0] - playerPos[0];
    float dirY = pushablePos[1] - playerPos[1];
    
    // Normalize direction
    float dirLength = sqrtf(dirX * dirX + dirY * dirY);
    if (dirLength > 0) 
    {
        dirX /= dirLength;
        dirY /= dirLength;
    }
    
    float dotProduct = worldMoveX * dirX + worldMoveY * dirY;

    return dotProduct > 0.3f; 
}

/* Check if pushable object is supported from below */
bool isPushableSupported(Entity* pushable, Entity* map)
{
    if (!map) return false;
    
    float pushablePos[3] = 
    {
        pushable->pushable.position[0],
        pushable->pushable.position[1],
        pushable->pushable.position[2]
    };
    float pushableSize[3] = 
    {
        pushable->pushable.size[0] / 2.0f,
        pushable->pushable.size[1] / 2.0f,
        pushable->pushable.size[2] / 2.0f
    };
    
    // Check area below the pushable object
    float checkMin[3] = 
    {
        pushablePos[0] - pushableSize[0] + skinWidth,
        pushablePos[1] - pushableSize[1] + skinWidth,
        pushablePos[2] - pushableSize[2] - pushableFallThreshold
    };
    float checkMax[3] = 
    {
        pushablePos[0] + pushableSize[0] - skinWidth,
        pushablePos[1] + pushableSize[1] - skinWidth,
        pushablePos[2] - pushableSize[2] + skinWidth
    };
    
    for (Entity* entity = map; entity != NULL; entity = entity->next) 
    {
        if (entity == pushable) continue; 
        
        if (entity->type == ENTITY_BRUSH || entity->type == ENTITY_PUSHABLE) 
        {
            float entityMin[3], entityMax[3];
            if (entity->type == ENTITY_BRUSH) 
            {
                entityMin[0] = entity->brush.position[0] - entity->brush.size[0] / 2;
                entityMin[1] = entity->brush.position[1] - entity->brush.size[1] / 2;
                entityMin[2] = entity->brush.position[2] - entity->brush.size[2] / 2;
                entityMax[0] = entity->brush.position[0] + entity->brush.size[0] / 2;
                entityMax[1] = entity->brush.position[1] + entity->brush.size[1] / 2;
                entityMax[2] = entity->brush.position[2] + entity->brush.size[2] / 2;
            } 
            else 
            {
                entityMin[0] = entity->pushable.position[0] - entity->pushable.size[0] / 2;
                entityMin[1] = entity->pushable.position[1] - entity->pushable.size[1] / 2;
                entityMin[2] = entity->pushable.position[2] - entity->pushable.size[2] / 2;
                entityMax[0] = entity->pushable.position[0] + entity->pushable.size[0] / 2;
                entityMax[1] = entity->pushable.position[1] + entity->pushable.size[1] / 2;
                entityMax[2] = entity->pushable.position[2] + entity->pushable.size[2] / 2;
            }

            if (checkMax[0] > entityMin[0] && checkMin[0] < entityMax[0] &&
                checkMax[1] > entityMin[1] && checkMin[1] < entityMax[1] &&
                checkMax[2] > entityMin[2] && checkMin[2] < entityMax[2]) 
            {
                return true; // Supported from below
            }
        }
    }
    
    return false; // No support below
}

/* Update pushables physics and collisions */
void updatePushables(Entity* map)
{
    if (enableNoClip) return;
    if (!map) return;
    
    for (Entity* entity = map; entity != NULL; entity = entity->next) 
    {
        if (entity->type != ENTITY_PUSHABLE) continue;
        
        if (!isPushableSupported(entity, map)) 
        {
            // Apply gravity
            float mass = entity->pushable.mass > 0 ? entity->pushable.mass : defaultPushableMass;

            entity->pushable.position[2] += pushableGravity / mass;
        }
    }
    
    for (Entity* entity1 = map; entity1 != NULL; entity1 = entity1->next) 
    {
        if (entity1->type != ENTITY_PUSHABLE) continue;
        
        for (Entity* entity2 = map; entity2 != NULL; entity2 = entity2->next) 
        {
            if (entity1 == entity2) continue;
            
            if (entity2->type != ENTITY_BRUSH && entity2->type != ENTITY_PUSHABLE) continue;
            
            float pos1[3] = 
            {
                entity1->pushable.position[0],
                entity1->pushable.position[1],
                entity1->pushable.position[2]
            };
            float size1[3] = 
            {
                entity1->pushable.size[0] / 2.0f,
                entity1->pushable.size[1] / 2.0f,
                entity1->pushable.size[2] / 2.0f
            };
            
            float pos2[3], size2[3];
            if (entity2->type == ENTITY_BRUSH)
            {
                pos2[0] = entity2->brush.position[0];
                pos2[1] = entity2->brush.position[1];
                pos2[2] = entity2->brush.position[2];
                size2[0] = entity2->brush.size[0] / 2.0f;
                size2[1] = entity2->brush.size[1] / 2.0f;
                size2[2] = entity2->brush.size[2] / 2.0f;
            } 
            else 
            {
                pos2[0] = entity2->pushable.position[0];
                pos2[1] = entity2->pushable.position[1];
                pos2[2] = entity2->pushable.position[2];
                size2[0] = entity2->pushable.size[0] / 2.0f;
                size2[1] = entity2->pushable.size[1] / 2.0f;
                size2[2] = entity2->pushable.size[2] / 2.0f;
            }
            
            if (checkAABBCollision(pos1, size1, pos2, size2)) 
            {
                float penetrationX = (size1[0] + size2[0]) - fabs(pos1[0] - pos2[0]);
                float penetrationY = (size1[1] + size2[1]) - fabs(pos1[1] - pos2[1]);
                float penetrationZ = (size1[2] + size2[2]) - fabs(pos1[2] - pos2[2]);
                
                if (penetrationX < penetrationY && penetrationX < penetrationZ) 
                {
                    if (pos1[0] < pos2[0]) 
                        entity1->pushable.position[0] -= penetrationX + skinWidth;
                    else 
                        entity1->pushable.position[0] += penetrationX + skinWidth;
                } 
                else if (penetrationY < penetrationZ) 
                {
                    if (pos1[1] < pos2[1]) 
                        entity1->pushable.position[1] -= penetrationY + skinWidth;
                    else 
                        entity1->pushable.position[1] += penetrationY + skinWidth;
                } 
                else 
                {
                    if (pos1[2] < pos2[2]) 
                    {
                        entity1->pushable.position[2] -= penetrationZ + skinWidth;
                    }
                    else 
                    {
                        entity1->pushable.position[2] += penetrationZ + skinWidth;
                    }
                }
            }
        }
    }
}

/* Check if player is standing on ground */
void checkGrounded(Entity* map) 
{
    if (enableNoClip) 
    {
        isGrounded = false;

        return;
    }
    
    if (!map) 
    {
        isGrounded = (playerZ <= 3.0f);

        return;
    }

    float rayStart[3] = {playerX, playerY, playerZ - playerFeetOffset};
    float rayEnd[3] = {playerX, playerY, playerZ - playerFeetOffset - 0.2f};
    
    isGrounded = false;

    for (Entity* entity = map; entity != NULL; entity = entity->next) 
    {
        if (entity->type != ENTITY_BRUSH && entity->type != ENTITY_PUSHABLE) continue;

        float brushMin[3], brushMax[3];
        if (entity->type == ENTITY_BRUSH) 
        {
            brushMin[0] = entity->brush.position[0] - entity->brush.size[0] / 2;
            brushMin[1] = entity->brush.position[1] - entity->brush.size[1] / 2;
            brushMin[2] = entity->brush.position[2] - entity->brush.size[2] / 2;
            brushMax[0] = entity->brush.position[0] + entity->brush.size[0] / 2;
            brushMax[1] = entity->brush.position[1] + entity->brush.size[1] / 2;
            brushMax[2] = entity->brush.position[2] + entity->brush.size[2] / 2;
        } 
        else 
        {
            brushMin[0] = entity->pushable.position[0] - entity->pushable.size[0] / 2;
            brushMin[1] = entity->pushable.position[1] - entity->pushable.size[1] / 2;
            brushMin[2] = entity->pushable.position[2] - entity->pushable.size[2] / 2;
            brushMax[0] = entity->pushable.position[0] + entity->pushable.size[0] / 2;
            brushMax[1] = entity->pushable.position[1] + entity->pushable.size[1] / 2;
            brushMax[2] = entity->pushable.position[2] + entity->pushable.size[2] / 2;
        }

        if (rayStart[0] >= brushMin[0] && rayStart[0] <= brushMax[0] &&
            rayStart[1] >= brushMin[1] && rayStart[1] <= brushMax[1] &&
            rayStart[2] >= brushMin[2] && rayEnd[2] <= brushMax[2]) 
        {
            isGrounded = true;

            break;
        }
    }
}

/* Check and resolve all player collisions with map */
void checkPlayerCollisions(Entity* map) 
{
    if (enableNoClip) return;
    
    if (!map || !physicsEnabled) return;

    float playerMin[3] = 
    {
        playerX - playerWidth,
        playerY - playerDepth,
        playerZ - playerFeetOffset
    };
    float playerMax[3] = 
    {
        playerX + playerWidth,
        playerY + playerDepth,
        playerZ + playerHeight
    };

    for (Entity* entity = map; entity != NULL; entity = entity->next) 
    {
        if (entity->type != ENTITY_BRUSH && entity->type != ENTITY_PUSHABLE) continue;

        float brushMin[3], brushMax[3];

        if (entity->type == ENTITY_BRUSH) 
        {
            brushMin[0] = entity->brush.position[0] - entity->brush.size[0] / 2;
            brushMin[1] = entity->brush.position[1] - entity->brush.size[1] / 2;
            brushMin[2] = entity->brush.position[2] - entity->brush.size[2] / 2;
            brushMax[0] = entity->brush.position[0] + entity->brush.size[0] / 2;
            brushMax[1] = entity->brush.position[1] + entity->brush.size[1] / 2;
            brushMax[2] = entity->brush.position[2] + entity->brush.size[2] / 2;
        } 
        else 
        {
            brushMin[0] = entity->pushable.position[0] - entity->pushable.size[0] / 2;
            brushMin[1] = entity->pushable.position[1] - entity->pushable.size[1] / 2;
            brushMin[2] = entity->pushable.position[2] - entity->pushable.size[2] / 2;
            brushMax[0] = entity->pushable.position[0] + entity->pushable.size[0] / 2;
            brushMax[1] = entity->pushable.position[1] + entity->pushable.size[1] / 2;
            brushMax[2] = entity->pushable.position[2] + entity->pushable.size[2] / 2;
        }

        // Check for collision
        if (playerMax[0] > brushMin[0] && playerMin[0] < brushMax[0] &&
            playerMax[1] > brushMin[1] && playerMin[1] < brushMax[1] &&
            playerMax[2] > brushMin[2] && playerMin[2] < brushMax[2]) 
        {
            if (entity->type == ENTITY_BRUSH) 
            {
                // Calculate overlap on each axis
                float overlapX = fmin(playerMax[0] - brushMin[0], brushMax[0] - playerMin[0]);
                float overlapY = fmin(playerMax[1] - brushMin[1], brushMax[1] - playerMin[1]);
                float overlapZ = fmin(playerMax[2] - brushMin[2], brushMax[2] - playerMin[2]);

                // Resolve along axis with minimum overlap
                if (overlapX < overlapY && overlapX < overlapZ) 
                {
                    if (playerX < entity->brush.position[0]) 
                    {
                        playerX = brushMin[0] - playerWidth - skinWidth;
                    } 
                    else 
                    {
                        playerX = brushMax[0] + playerWidth + skinWidth;
                    }
                } 
                else if (overlapY < overlapZ) 
                {
                    if (playerY < entity->brush.position[1]) 
                    {
                        playerY = brushMin[1] - playerDepth - skinWidth;
                    } 
                    else 
                    {
                        playerY = brushMax[1] + playerDepth + skinWidth;
                    }
                } 
                else 
                {
                    if (playerZ < entity->brush.position[2]) 
                    {
                        playerZ = brushMin[2] - playerHeight - skinWidth;
                        if (verticalVelocity > 0) verticalVelocity = 0;
                    } 
                    else 
                    {
                        playerZ = brushMax[2] + playerFeetOffset + skinWidth;
                        isGrounded = true;

                        if (verticalVelocity < 0) verticalVelocity = 0;
                    }
                }
            } 
            else if (entity->type == ENTITY_PUSHABLE) 
            {
                float overlapX = fmin(playerMax[0] - brushMin[0], brushMax[0] - playerMin[0]);
                float overlapY = fmin(playerMax[1] - brushMin[1], brushMax[1] - playerMin[1]);
                float overlapZ = fmin(playerMax[2] - brushMin[2], brushMax[2] - playerMin[2]);

                if (overlapX < overlapY && overlapX < overlapZ) 
                {
                    if (playerX < entity->pushable.position[0]) 
                    {
                        playerX = brushMin[0] - playerWidth - skinWidth;
                    } 
                    else 
                    {
                        playerX = brushMax[0] + playerWidth + skinWidth;
                    }

                    handlePushableCollision(entity);
                } 
                else if (overlapY < overlapZ) 
                {
                    if (playerY < entity->pushable.position[1]) 
                    {
                        playerY = brushMin[1] - playerDepth - skinWidth;
                    } 
                    else 
                    {
                        playerY = brushMax[1] + playerDepth + skinWidth;
                    }

                    handlePushableCollision(entity);
                } 
                else 
                {
                    if (playerZ < entity->pushable.position[2]) 
                    {
                        playerZ = brushMin[2] - playerHeight - skinWidth;

                        if (verticalVelocity > 0) verticalVelocity = 0;
                    } 
                    else 
                    {
                        playerZ = brushMax[2] + playerFeetOffset + skinWidth;

                        isGrounded = true;

                        if (verticalVelocity < 0) verticalVelocity = 0;
                    }
                }
            }

            // Update player bounds after resolution
            playerMin[0] = playerX - playerWidth;
            playerMax[0] = playerX + playerWidth;
            playerMin[1] = playerY - playerDepth;
            playerMax[1] = playerY + playerDepth;
            playerMin[2] = playerZ - playerFeetOffset;
            playerMax[2] = playerZ + playerHeight;
        }
    }
    
    updatePushables(map);
}

/* Find valid spawn position avoiding collisions */
void validateSpawnPosition(Entity* map) 
{
    if (enableNoClip) return;

    float playerSize[3] = {playerWidth, playerHeight, playerDepth};
    float playerPos[3] = {playerX, playerY, playerZ};
    
    const int maxAttempts = 100;
    int attempts = 0;
    
    while (attempts < maxAttempts) 
    {
        bool collided = false;
        
        for (Entity* entity = map; entity != NULL; entity = entity->next) 
        {
            if (entity->type == ENTITY_BRUSH || entity->type == ENTITY_PUSHABLE) 
            {
                float brushSize[3], brushPos[3];

                if (entity->type == ENTITY_BRUSH) 
                {
                    brushSize[0] = entity->brush.size[0] / 2.0f;
                    brushSize[1] = entity->brush.size[1] / 2.0f;
                    brushSize[2] = entity->brush.size[2] / 2.0f;
                    brushPos[0] = entity->brush.position[0];
                    brushPos[1] = entity->brush.position[1];
                    brushPos[2] = entity->brush.position[2];
                } 
                else 
                {
                    brushSize[0] = entity->pushable.size[0] / 2.0f;
                    brushSize[1] = entity->pushable.size[1] / 2.0f;
                    brushSize[2] = entity->pushable.size[2] / 2.0f;
                    brushPos[0] = entity->pushable.position[0];
                    brushPos[1] = entity->pushable.position[1];
                    brushPos[2] = entity->pushable.position[2];
                }
                
                if (checkAABBCollision(playerPos, playerSize, brushPos, brushSize)) 
                {
                    collided = true;

                    playerPos[2] += 0.1f;

                    break;
                }
            }
        }
        
        if (!collided) 
        {
            playerX = playerPos[0];
            playerY = playerPos[1];
            playerZ = playerPos[2];
            return;
        }
        
        attempts++;
    }
    
    playerZ = 3.0f + playerHeight;
}

/* Drawing AABB player collision wireframe */
void drawPlayerWireframeAABB(void)
{
    if (collisionWireframe == 1)
    {
        glDisable(GL_TEXTURE_2D);
        glColor3f(wireframeColorRed, wireframeColorGreen, wireframeColorBlue);
        glLineWidth(2.0f);
        
        float minX = playerX - playerWidth;
        float maxX = playerX + playerWidth;
        float minY = playerY - playerDepth;
        float maxY = playerY + playerDepth;
        float minZ = playerZ - playerFeetOffset;
        float maxZ = playerZ + playerHeight;
        
        glBegin(GL_LINES);
        
        glVertex3f(minX, minY, minZ); glVertex3f(maxX, minY, minZ);
        glVertex3f(maxX, minY, minZ); glVertex3f(maxX, maxY, minZ);
        glVertex3f(maxX, maxY, minZ); glVertex3f(minX, maxY, minZ);
        glVertex3f(minX, maxY, minZ); glVertex3f(minX, minY, minZ);
        
        glVertex3f(minX, minY, maxZ); glVertex3f(maxX, minY, maxZ);
        glVertex3f(maxX, minY, maxZ); glVertex3f(maxX, maxY, maxZ);
        glVertex3f(maxX, maxY, maxZ); glVertex3f(minX, maxY, maxZ);
        glVertex3f(minX, maxY, maxZ); glVertex3f(minX, minY, maxZ);
        
        glVertex3f(minX, minY, minZ); glVertex3f(minX, minY, maxZ);
        glVertex3f(maxX, minY, minZ); glVertex3f(maxX, minY, maxZ);
        glVertex3f(maxX, maxY, minZ); glVertex3f(maxX, maxY, maxZ);
        glVertex3f(minX, maxY, minZ); glVertex3f(minX, maxY, maxZ);
        
        glEnd();
        
        glEnable(GL_TEXTURE_2D);
        glColor3f(1.0f, 1.0f, 1.0f);
    }
}

/* Drawing AABB collision wireframe for entities */
void drawEntityWireframeAABB(Entity* entity)
{
    if (collisionWireframe == 1 && (entity->type == ENTITY_BRUSH || entity->type == ENTITY_PUSHABLE))
    {
        glDisable(GL_TEXTURE_2D);

        glColor3f(wireframeColorRed, wireframeColorGreen, wireframeColorBlue);
        glLineWidth(1.0f);

        float minX, maxX, minY, maxY, minZ, maxZ;
        
        if (entity->type == ENTITY_BRUSH) 
        {
            minX = entity->brush.position[0] - entity->brush.size[0] / 2;
            maxX = entity->brush.position[0] + entity->brush.size[0] / 2;
            minY = entity->brush.position[1] - entity->brush.size[1] / 2;
            maxY = entity->brush.position[1] + entity->brush.size[1] / 2;
            minZ = entity->brush.position[2] - entity->brush.size[2] / 2;
            maxZ = entity->brush.position[2] + entity->brush.size[2] / 2;
        } 
        else 
        {
            minX = entity->pushable.position[0] - entity->pushable.size[0] / 2;
            maxX = entity->pushable.position[0] + entity->pushable.size[0] / 2;
            minY = entity->pushable.position[1] - entity->pushable.size[1] / 2;
            maxY = entity->pushable.position[1] + entity->pushable.size[1] / 2;
            minZ = entity->pushable.position[2] - entity->pushable.size[2] / 2;
            maxZ = entity->pushable.position[2] + entity->pushable.size[2] / 2;
        }

        glBegin(GL_LINES);

            glVertex3f(minX, minY, minZ); glVertex3f(maxX, minY, minZ);
            glVertex3f(maxX, minY, minZ); glVertex3f(maxX, maxY, minZ);
            glVertex3f(maxX, maxY, minZ); glVertex3f(minX, maxY, minZ);
            glVertex3f(minX, maxY, minZ); glVertex3f(minX, minY, minZ);

            glVertex3f(minX, minY, maxZ); glVertex3f(maxX, minY, maxZ);
            glVertex3f(maxX, minY, maxZ); glVertex3f(maxX, maxY, maxZ);
            glVertex3f(maxX, maxY, maxZ); glVertex3f(minX, maxY, maxZ);
            glVertex3f(minX, maxY, maxZ); glVertex3f(minX, minY, maxZ);

            glVertex3f(minX, minY, minZ); glVertex3f(minX, minY, maxZ);
            glVertex3f(maxX, minY, minZ); glVertex3f(maxX, minY, maxZ);
            glVertex3f(maxX, maxY, minZ); glVertex3f(maxX, maxY, maxZ);
            glVertex3f(minX, maxY, minZ); glVertex3f(minX, maxY, maxZ);

        glEnd();

        glEnable(GL_TEXTURE_2D);
        glColor3f(1.0f, 1.0f, 1.0f);
    }
}

/* Check camera collision with brushes */
bool checkCameraCollision(float camPos[3], Entity* map)
{
    if (enableNoClip) return false;

    if (!map) return false;

    // Camera collision bounds (smaller than player)
    float camWidth = 0.1f;
    float camHeight = 0.1f;
    float camDepth = 0.1f;

    float camMin[3] = 
    {
        camPos[0] - camWidth,
        camPos[1] - camDepth,
        camPos[2] - camHeight
    };
    float camMax[3] = 
    {
        camPos[0] + camWidth,
        camPos[1] + camDepth,
        camPos[2] + camHeight
    };

    for (Entity* entity = map; entity != NULL; entity = entity->next) 
    {
        if (entity->type != ENTITY_BRUSH && entity->type != ENTITY_PUSHABLE) continue;

        float brushMin[3], brushMax[3];

        if (entity->type == ENTITY_BRUSH) 
        {
            brushMin[0] = entity->brush.position[0] - entity->brush.size[0] / 2;
            brushMin[1] = entity->brush.position[1] - entity->brush.size[1] / 2;
            brushMin[2] = entity->brush.position[2] - entity->brush.size[2] / 2;
            brushMax[0] = entity->brush.position[0] + entity->brush.size[0] / 2;
            brushMax[1] = entity->brush.position[1] + entity->brush.size[1] / 2;
            brushMax[2] = entity->brush.position[2] + entity->brush.size[2] / 2;
        } 
        else 
        {
            brushMin[0] = entity->pushable.position[0] - entity->pushable.size[0] / 2;
            brushMin[1] = entity->pushable.position[1] - entity->pushable.size[1] / 2;
            brushMin[2] = entity->pushable.position[2] - entity->pushable.size[2] / 2;
            brushMax[0] = entity->pushable.position[0] + entity->pushable.size[0] / 2;
            brushMax[1] = entity->pushable.position[1] + entity->pushable.size[1] / 2;
            brushMax[2] = entity->pushable.position[2] + entity->pushable.size[2] / 2;
        }

        // Check for collision
        if (camMax[0] > brushMin[0] && camMin[0] < brushMax[0] &&
            camMax[1] > brushMin[1] && camMin[1] < brushMax[1] &&
            camMax[2] > brushMin[2] && camMin[2] < brushMax[2]) 
        {
            return true; // Camera is colliding with brush
        }
    }

    return false;
}

/* Adjust camera position to avoid collision */
void adjustCameraPosition(float playerPos[3], float* cameraOffset, Entity* map)
{
    if (enableNoClip) return;
    
    const float maxCameraDistance = 5.0f; // Maximum camera distance from player
    const float minCameraDistance = 1.0f; // Minimum camera distance from player
    const float step = 0.1f;
    
    float originalOffset = *cameraOffset;
    float testOffset = originalOffset;
    
    // Calculate camera position
    float camPos[3] = 
    {
        playerPos[0],
        playerPos[1], 
        playerPos[2] - testOffset
    };
    
    // If camera is not colliding at current offset, we're good
    if (!checkCameraCollision(camPos, map)) 
    {
        return;
    }
    
    // Try to find a safe camera position by moving it closer to player
    bool foundSafePosition = false;

    for (testOffset = minCameraDistance; testOffset <= maxCameraDistance; testOffset += step) 
    {
        camPos[2] = playerPos[2] - testOffset;
        
        if (!checkCameraCollision(camPos, map)) 
        {
            *cameraOffset = testOffset;
            foundSafePosition = true;
            break;
        }
    }
    
    // If no safe position found, use minimum distance
    if (!foundSafePosition) 
    {
        *cameraOffset = minCameraDistance;
    }
}
