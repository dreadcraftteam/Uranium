// AABB collision and basic physics 
// By DREADCRAFT
// 

#include "base.h"

#include "defines.h"
#include "player.h"
#include "collision.h"

/* Collision constants */
const float playerFeetOffset = 5.0f;
const float skinWidth = 0.05f;

static int physicsEnabled = 1;

/* Check if two AABBs intersect */
int checkAABBCollision(float pos1[3], float size1[3], float pos2[3], float size2[3]) 
{
    return 
        fabs(pos1[0] - pos2[0]) < (size1[0] + size2[0]) &&
        fabs(pos1[1] - pos2[1]) < (size1[1] + size2[1]) &&
        fabs(pos1[2] - pos2[2]) < (size1[2] + size2[2]);
}

/* Handle collision response */
void handleCollision(Entity* brush) 
{
    float playerSize[3] = {playerWidth, playerHeight, playerDepth};
    float brushSize[3] = 
    {
        brush->brush.size[0] / 2.0f,
        brush->brush.size[1] / 2.0f,
        brush->brush.size[2] / 2.0f
    };
    
    float playerPos[3] = {x, y, z};
    float brushPos[3] = 
    {
        brush->brush.position[0],
        brush->brush.position[1],
        brush->brush.position[2]
    };
    
    float penetrationX = (playerWidth + brushSize[0]) - fabs(playerPos[0] - brushPos[0]);
    float penetrationY = (playerHeight + brushSize[1]) - fabs(playerPos[1] - brushPos[1]);
    float penetrationZ = (playerDepth + brushSize[2]) - fabs(playerPos[2] - brushPos[2]);
    
    if (penetrationX < penetrationY && penetrationX < penetrationZ) 
    {
        if (playerPos[0] < brushPos[0]) x -= penetrationX + 0.01f;
        else x += penetrationX + 0.01f;
    } 
    else if (penetrationY < penetrationZ) 
    {
        if (playerPos[1] < brushPos[1]) y -= penetrationY + 0.01f;
        else y += penetrationY + 0.01f;
    } 
    else 
    {
        if (playerPos[2] < brushPos[2]) 
        {
            z -= penetrationZ + 0.01f;

            if (verticalVelocity < 0) 
            {
                isGrounded = true;
                verticalVelocity = 0.0f;
            }
        } 
        else 
        {
            z += penetrationZ + 0.01f;

            if (verticalVelocity > 0) 
            {
                verticalVelocity = 0.0f;
            }
        }
    }
}

/* Checking the ground */
void checkGrounded(Entity* map) 
{
    if (!map) 
    {
        isGrounded = (z <= groundLevel);
        return;
    }

    float rayStart[3] = {x, y, z - playerFeetOffset};
    float rayEnd[3] = {x, y, z - playerFeetOffset - 0.2f};
    isGrounded = false;

    for (Entity* e = map; e != NULL; e = e->next) 
    {
        if (e->type != ENTITY_BRUSH) continue;

        float brushMin[3] = 
        {
            e->brush.position[0] - e->brush.size[0]/2,
            e->brush.position[1] - e->brush.size[1]/2,
            e->brush.position[2] - e->brush.size[2]/2
        };
        float brushMax[3] = 
        {
            e->brush.position[0] + e->brush.size[0]/2,
            e->brush.position[1] + e->brush.size[1]/2,
            e->brush.position[2] + e->brush.size[2]/2
        };

        if (rayStart[0] >= brushMin[0] && rayStart[0] <= brushMax[0] &&
            rayStart[1] >= brushMin[1] && rayStart[1] <= brushMax[1] &&
            rayStart[2] >= brushMin[2] && rayEnd[2] <= brushMax[2]) 
        {
            isGrounded = true;
            break;
        }
    }
}

/* Checking player collisions */
void checkPlayerCollisions(Entity* map) 
{
    if (!map || !physicsEnabled) return;

    float playerMin[3] = 
    {
        x - playerWidth,
        y - playerDepth,
        z - playerFeetOffset
    };
    float playerMax[3] = 
    {
        x + playerWidth,
        y + playerDepth,
        z + playerHeight
    };

    for (Entity* e = map; e != NULL; e = e->next) 
    {
        if (e->type != ENTITY_BRUSH) continue;

        float brushMin[3] = 
        {
            e->brush.position[0] - e->brush.size[0]/2,
            e->brush.position[1] - e->brush.size[1]/2,
            e->brush.position[2] - e->brush.size[2]/2
        };
        float brushMax[3] = 
        {
            e->brush.position[0] + e->brush.size[0]/2,
            e->brush.position[1] + e->brush.size[1]/2,
            e->brush.position[2] + e->brush.size[2]/2
        };

        if (playerMax[0] > brushMin[0] && playerMin[0] < brushMax[0] &&
            playerMax[1] > brushMin[1] && playerMin[1] < brushMax[1] &&
            playerMax[2] > brushMin[2] && playerMin[2] < brushMax[2]) 
            {
            
            float overlapX = fmin(playerMax[0] - brushMin[0], brushMax[0] - playerMin[0]);
            float overlapY = fmin(playerMax[1] - brushMin[1], brushMax[1] - playerMin[1]);
            float overlapZ = fmin(playerMax[2] - brushMin[2], brushMax[2] - playerMin[2]);

            if (overlapX < overlapY && overlapX < overlapZ) 
            {
                if (playerMax[0] - playerWidth < brushMin[0]) 
                {
                    x = brushMin[0] - playerWidth - skinWidth;
                } 
                else 
                {
                    x = brushMax[0] + playerWidth + skinWidth;
                }
            } 
            else if (overlapY < overlapZ) 
            {
                if (playerMax[1] - playerDepth < brushMin[1]) 
                {
                    y = brushMin[1] - playerDepth - skinWidth;
                } 
                else 
                {
                    y = brushMax[1] + playerDepth + skinWidth;
                }
            } 
            else 
            {
                if (playerMax[2] - playerHeight < brushMin[2]) 
                {
                    z = brushMin[2] - playerHeight - skinWidth;
                    if (verticalVelocity > 0) verticalVelocity = 0;
                } 
                else 
                {
                    z = brushMax[2] + playerFeetOffset + skinWidth;
                    isGrounded = true;
                    if (verticalVelocity < 0) verticalVelocity = 0;
                }
            }

            playerMin[0] = x - playerWidth;
            playerMax[0] = x + playerWidth;
            playerMin[1] = y - playerDepth;
            playerMax[1] = y + playerDepth;
            playerMin[2] = z - playerFeetOffset;
            playerMax[2] = z + playerHeight;
        }
    }
}

/* Validating spawn position */
void validateSpawnPosition(Entity* map) 
{
    float playerSize[3] = {playerWidth, playerHeight, playerDepth};
    float playerPos[3] = {x, y, z};
    int attempts = 0;
    const int maxAttempts = 100;
    
    while (attempts < maxAttempts) 
    {
        int collided = 0;
        
        for (Entity* e = map; e != NULL; e = e->next) 
        {
            if (e->type == ENTITY_BRUSH) 
            {
                float brushSize[3] = 
                {
                    e->brush.size[0] / 2.0f,
                    e->brush.size[1] / 2.0f,
                    e->brush.size[2] / 2.0f
                };
                float brushPos[3] = 
                {
                    e->brush.position[0],
                    e->brush.position[1],
                    e->brush.position[2]
                };
                
                if (checkAABBCollision(playerPos, playerSize, brushPos, brushSize)) 
                {
                    collided = 1;
                    playerPos[2] += 0.1f;
                    break;
                }
            }
        }
        
        if (!collided) 
        {
            x = playerPos[0];
            y = playerPos[1];
            z = playerPos[2];
            return;
        }
        
        attempts++;
    }
    
    z = groundLevel + playerHeight;
}
