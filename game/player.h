// Player controller for games
// By DREADCRAFT
//

#ifndef PLAYER_H
#define PLAYER_H

#include "base.h"

#include "GLFW/glfw3.h"

#include "defines.h"

/* General variables */
extern float cameraX;
extern float cameraZ;

extern float x;
extern float y;
extern float z;

/* Jump variables */
extern float verticalVelocity;

extern const float gravity;
extern float jumpForce;
extern const float groundLevel;

extern bool isGrounded;
extern bool jumpKeyPressed;

extern float mouseSensitivity;

/* AABB collision */
extern float playerWidth;
extern float playerHeight;
extern float playerDepth;

/* Initialize player with default values */
void playerInit();

/* Camera and player initialization */
void player();

/* Movement input */
void handleMovementInput(GLFWwindow* frame);

/* Mouse input */
void handleMouseInput(GLFWwindow* frame);

/* Collision functions */
void checkPlayerCollisions(Entity* map);
void checkGrounded(Entity* map);
void validateSpawnPosition(Entity* map);

/* Position setting */
void setPlayerPosition(float newX, float newY, float newZ);

#endif // PLAYER_H
