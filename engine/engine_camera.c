// Technically it's a "PlayerController"
// By DREADCRAFT
//

#include "base.h"

#include "GLFW/glfw3.h"

#include "engine_variables.h"
#include "engine_camera.h"

/* General variables */
float cameraX = 85.0f;
float cameraZ = 0.0f;

float x = 0.0f;
float y = 0.0f;
float z = 3.0f;

/* Jump variables */
float verticalVelocity = 0.0f;

const float gravity = -0.005f;
float jumpForce = 0.15f;
const float groundLevel = 3.0f;

bool isGrounded = false;
bool jumpKeyPressed = false;

float mouseSensitivity = 0.1f;

/* Technically it's a PlayerController */
void camera()
{
    /* Apply jump physics */
    verticalVelocity += gravity;
    z += verticalVelocity;

    /* Ground collision */
    if (z <= groundLevel)
    {
        z = groundLevel;
        verticalVelocity = 0.0f;
        isGrounded = true;
        jumpForce = 0.15f;
    }
    else
    {
        isGrounded = false;
        jumpForce = 0.0f;
    }

    /* Camera position */
    glRotatef(-cameraX, 1, 0, 0);
    glRotatef(-cameraZ, 0, 0, 1);

    glTranslatef(-x, -y, -z);
}

void handleJumpInput(GLFWwindow* frame)
{
    if (glfwGetKey(frame, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (!jumpKeyPressed && groundLevel)
        {
            verticalVelocity = jumpForce;
            isGrounded = false;
        }
        jumpKeyPressed = true;
    }
    else
    {
        jumpKeyPressed = false;
    }
}
