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
    float brushSize[3] = {
        brush->brush.size[0] / 2.0f,
        brush->brush.size[1] / 2.0f,
        brush->brush.size[2] / 2.0f
    };
    
    float playerPos[3] = {playerX, playerY, playerZ};
    float brushPos[3] = {
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
        // Fallback to simple ground check if no map
        isGrounded = (playerZ <= 3.0f); // groundLevel constant

        return;
    }

    float rayStart[3] = {playerX, playerY, playerZ - playerFeetOffset};
    float rayEnd[3] = {playerX, playerY, playerZ - playerFeetOffset - 0.2f};
    isGrounded = false;

    for (Entity* entity = map; entity != NULL; entity = entity->next) 
    {
        if (entity->type != ENTITY_BRUSH) continue;

        float brushMin[3] = 
        {
            entity->brush.position[0] - entity->brush.size[0] / 2,
            entity->brush.position[1] - entity->brush.size[1] / 2,
            entity->brush.position[2] - entity->brush.size[2] / 2
        };
        float brushMax[3] = 
        {
            entity->brush.position[0] + entity->brush.size[0] / 2,
            entity->brush.position[1] + entity->brush.size[1] / 2,
            entity->brush.position[2] + entity->brush.size[2] / 2
        };

        // Check if ray intersects with brush
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

    float playerMin[3] = {
        playerX - playerWidth,
        playerY - playerDepth,
        playerZ - playerFeetOffset
    };
    float playerMax[3] = {
        playerX + playerWidth,
        playerY + playerDepth,
        playerZ + playerHeight
    };

    for (Entity* entity = map; entity != NULL; entity = entity->next) 
    {
        if (entity->type != ENTITY_BRUSH) continue;

        float brushMin[3] = {
            entity->brush.position[0] - entity->brush.size[0] / 2,
            entity->brush.position[1] - entity->brush.size[1] / 2,
            entity->brush.position[2] - entity->brush.size[2] / 2
        };
        float brushMax[3] = {
            entity->brush.position[0] + entity->brush.size[0] / 2,
            entity->brush.position[1] + entity->brush.size[1] / 2,
            entity->brush.position[2] + entity->brush.size[2] / 2
        };

        // Check for collision
        if (playerMax[0] > brushMin[0] && playerMin[0] < brushMax[0] &&
            playerMax[1] > brushMin[1] && playerMin[1] < brushMax[1] &&
            playerMax[2] > brushMin[2] && playerMin[2] < brushMax[2]) 
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

            // Update player bounds after resolution
            playerMin[0] = playerX - playerWidth;
            playerMax[0] = playerX + playerWidth;
            playerMin[1] = playerY - playerDepth;
            playerMax[1] = playerY + playerDepth;
            playerMin[2] = playerZ - playerFeetOffset;
            playerMax[2] = playerZ + playerHeight;
        }
    }
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
            if (entity->type == ENTITY_BRUSH) 
            {
                float brushSize[3] = {
                    entity->brush.size[0] / 2.0f,
                    entity->brush.size[1] / 2.0f,
                    entity->brush.size[2] / 2.0f
                };
                float brushPos[3] = {
                    entity->brush.position[0],
                    entity->brush.position[1],
                    entity->brush.position[2]
                };
                
                if (checkAABBCollision(playerPos, playerSize, brushPos, brushSize)) 
                {
                    collided = true;
                    playerPos[2] += 0.1f; // Move up
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
    
    // Fallback position if no valid spawn found
    playerZ = 3.0f + playerHeight; // groundLevel + playerHeight
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
    if (collisionWireframe == 1 && entity->type == ENTITY_BRUSH)
    {
        glDisable(GL_TEXTURE_2D);

        glColor3f(wireframeColorRed, wireframeColorGreen, wireframeColorBlue);
        glLineWidth(1.0f);

        float minX = entity->brush.position[0] - entity->brush.size[0] / 2;
        float maxX = entity->brush.position[0] + entity->brush.size[0] / 2;
        float minY = entity->brush.position[1] - entity->brush.size[1] / 2;
        float maxY = entity->brush.position[1] + entity->brush.size[1] / 2;
        float minZ = entity->brush.position[2] - entity->brush.size[2] / 2;
        float maxZ = entity->brush.position[2] + entity->brush.size[2] / 2;

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
    float camWidth = 0.3f;
    float camHeight = 0.3f;
    float camDepth = 0.3f;

    float camMin[3] = {
        camPos[0] - camWidth,
        camPos[1] - camDepth,
        camPos[2] - camHeight
    };
    float camMax[3] = {
        camPos[0] + camWidth,
        camPos[1] + camDepth,
        camPos[2] + camHeight
    };

    for (Entity* entity = map; entity != NULL; entity = entity->next) 
    {
        if (entity->type != ENTITY_BRUSH) continue;

        float brushMin[3] = {
            entity->brush.position[0] - entity->brush.size[0] / 2,
            entity->brush.position[1] - entity->brush.size[1] / 2,
            entity->brush.position[2] - entity->brush.size[2] / 2
        };
        float brushMax[3] = {
            entity->brush.position[0] + entity->brush.size[0] / 2,
            entity->brush.position[1] + entity->brush.size[1] / 2,
            entity->brush.position[2] + entity->brush.size[2] / 2
        };

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
