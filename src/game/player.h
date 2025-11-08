// Player controller for games on Uranium (engine)
// By DREADCRAFT
//

#ifndef PLAYER_H
#define PLAYER_H

#include "base.h"

#include "GLFW/glfw3.h"

#include "defines.h"
#include "../engine/umap.h"

/* Physics */
extern float verticalVelocity;
extern const float gravity;
extern float jumpForce;
extern bool isGrounded;

extern bool isCrouching; 
extern const float crouchHeight;
extern const float normalHeight;

/* Player properties */
extern float playerWidth; 
extern float playerHeight; 
extern float playerDepth;
extern float mouseSensitivity;
extern int health;
extern int armor;

/* Player state */
extern bool isPlayerDead;
extern float deathCameraOffset;
extern const float maxDeathCameraOffset;

/* Initialization */
void playerInit(void);

/* Main player update */
void playerUpdate(void);

/* Input handling */
void handleMovementInput(void);
void handleMouseInput(GLFWwindow* window);
void handleCrouchInput(void);
void handleJumpInput(void);

/* Death handling */
void handlePlayerDeath(void);
void handleDeathInput(void);
void respawnPlayer(void);

/* Collision detection */
void checkPlayerCollisions(Entity* map);
void checkGrounded(Entity* map);
void validateSpawnPosition(Entity* map);

/* Utilities */
void setPlayerPosition(float x, float y, float z);

#endif // PLAYER_H
