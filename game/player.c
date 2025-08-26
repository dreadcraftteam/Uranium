// Player controller for games 
// By DREADCRAFT
//

#include "base.h"

#include "GLFW/glfw3.h"

#include "defines.h"
#include "player.h"
#include "collision.h"

/* General variables */
float cameraX = 85.0f;
float cameraZ = 0.0f;

/* Position */
float x = 0.0f;
float y = 0.0f;
float z = 5.0f;

/* Jump variables */
float verticalVelocity = 0.0f;

const float gravity = -0.005f;
float jumpForce = 0.15f;
const float groundLevel = 3.0f;

bool isGrounded = false;
bool jumpKeyPressed = false;
bool cursorHidden = false;

float mouseSensitivity = 0.1f;

/* Camera bob variables */
float cameraBobOffset = 0.0f;
float cameraBobTime = 0.0f;
const float cameraBobSpeed = 0.1f;
const float cameraBobAmount = 0.4f;
bool wasMoving = false;

/* AABB collision */
float playerWidth = 2.5f;
float playerHeight = 7.0f;
float playerDepth = 2.5f;

/* Mouse input handling */
bool firstMouseInput = true;
double lastMouseX = 0.0;
double lastMouseY = 0.0;

static int physicsEnabled = 1;

/* Initialize player with default values */
void playerInit()
{
    cameraX = 85.0f;
    cameraZ = 0.0f;
    x = 0.0f;
    y = 0.0f;
    z = 5.0f;
    verticalVelocity = 0.0f;
    isGrounded = false;
    jumpKeyPressed = false;
    cameraBobOffset = 0.0f;
    cameraBobTime = 0.0f;
    wasMoving = false;
    firstMouseInput = true;

    if (glfwGetCurrentContext())
    {
        int centerX = width / 2;
        int centerY = height / 2;
        glfwSetCursorPos(glfwGetCurrentContext(), centerX, centerY);
    }
}

/* Technically it's a PlayerController */
void player()
{
    if (!isGrounded) 
    {
        verticalVelocity += gravity;
    } 
    else if (verticalVelocity < 0) 
    {
        verticalVelocity = 0;
    }

    z += verticalVelocity;

    checkPlayerCollisions(mapLoad);
    checkGrounded(mapLoad);

    // Apply camera bob effect
    float bobOffset = 0.0f;
    if (wasMoving && isGrounded) {
        cameraBobTime += cameraBobSpeed;
        bobOffset = sinf(cameraBobTime) * cameraBobAmount;
    } else {
        // Smoothly return to normal position when not moving
        if (cameraBobOffset != 0.0f) {
            cameraBobOffset *= 0.9f;
            if (fabsf(cameraBobOffset) < 0.01f) {
                cameraBobOffset = 0.0f;
            }
        }
        bobOffset = cameraBobOffset;
    }

    glRotatef(-cameraX + bobOffset, 1, 0, 0); // Apply vertical bob to camera rotation
    glRotatef(-cameraZ, 0, 0, 1);
    glTranslatef(-x, -y, -z);
}

/* Setting player position */
void setPlayerPosition(float newX, float newY, float newZ) 
{
    physicsEnabled = 0;
    x = newX;
    y = newY;
    z = newZ;
    physicsEnabled = 1;
    
    checkPlayerCollisions(mapLoad);
    checkGrounded(mapLoad);
}

/* Movement input */
void handleMovementInput(GLFWwindow* frame) 
{
    float angle = -cameraZ / 180 * PI;
    float speedX = 0;
    float speedY = 0;
    float movementSpeed = 0.1f; 
    float slowDownFactor = 0.4f;
    float currentSpeed = movementSpeed;

    bool isSlowMode = KEY_DOWN(INPUT_KEY_LEFT_SHIFT) || KEY_DOWN(INPUT_KEY_RIGHT_SHIFT);

    if (isSlowMode) 
    {
        currentSpeed *= slowDownFactor;
    }

    glfwSetInputMode(frame, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    cursorHidden = true;

    /* Check key states using new input system */
    bool w_pressed = KEY_DOWN(INPUT_KEY_W);
    bool s_pressed = KEY_DOWN(INPUT_KEY_S);
    bool a_pressed = KEY_DOWN(INPUT_KEY_A);
    bool d_pressed = KEY_DOWN(INPUT_KEY_D);

    /* Handle camera rotation */
    if (KEY_DOWN(INPUT_KEY_UP)) 
    {
        cameraX = (cameraX + 1) > 180 ? 180 : cameraX + 1;
    }
    if (KEY_DOWN(INPUT_KEY_DOWN)) 
    {
        cameraX = (cameraX - 1) < 0 ? 0 : cameraX - 1;
    }
    if (KEY_DOWN(INPUT_KEY_LEFT)) 
    {
        cameraZ++;
    }
    if (KEY_DOWN(INPUT_KEY_RIGHT)) 
    {
        cameraZ--;
    }

    /* Handle movement */
    if (w_pressed && !s_pressed) 
    {
        speedY += currentSpeed;
    }
    if (s_pressed && !w_pressed) 
    {
        speedY -= currentSpeed;
    }
    if (a_pressed && !d_pressed) 
    {
        speedX -= currentSpeed;
    }
    if (d_pressed && !a_pressed) 
    {
        speedX += currentSpeed;
    }

    /* Check if player is moving */
    bool isMoving = (speedX != 0 || speedY != 0);
    wasMoving = isMoving;

    /* Normalize diagonal movement */
    if (speedX != 0 && speedY != 0) 
    {
        float len = sqrtf(speedX * speedX + speedY * speedY);
        speedX = speedX / len * currentSpeed;
        speedY = speedY / len * currentSpeed;
    }

    /* Apply movement */
    if (isMoving) 
    {
        x += sinf(angle) * speedY + cosf(angle) * speedX;
        y += cosf(angle) * speedY - sinf(angle) * speedX;
    }

    /* Jump input */
    if (KEY_DOWN(INPUT_KEY_SPACE))
    {
        if (!jumpKeyPressed && isGrounded)
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

/* Mouse input */
void handleMouseInput(GLFWwindow* frame)
{
    double mouseX, mouseY;
    inputSystemGetMousePosition(&mouseX, &mouseY);

    int centerX = width / 2;
    int centerY = height / 2;

    // Ignore first mouse input to avoid random cursor position affecting camera
    if (firstMouseInput)
    {
        lastMouseX = mouseX;
        lastMouseY = mouseY;
        firstMouseInput = false;
        glfwSetCursorPos(frame, centerX, centerY);
        return;
    }

    int deltaX = (int)(mouseX - lastMouseX);
    int deltaY = (int)(mouseY - lastMouseY);
    
    // Only process if there was actual movement
    if (deltaX != 0 || deltaY != 0) 
    {
        cameraZ -= deltaX * mouseSensitivity;
        cameraX -= deltaY * mouseSensitivity;
            
        if (cameraX > 180.0f) cameraX = 180.0f;
        if (cameraX < 0.0f) cameraX = 0.0f;
            
        glfwSetCursorPos(frame, centerX, centerY);
    }

    lastMouseX = centerX; // Store center position for next frame
    lastMouseY = centerY;
}