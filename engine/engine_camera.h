// Technically it's a "PlayerController"
// By DREADCRAFT
//

#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include "base.h"

#include "GLFW/glfw3.h"

#include "engine_variables.h"

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

void camera();
void handleJumpInput();

#endif // ENGINE_CAMERA_H
