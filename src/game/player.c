// Player controller for games on Uranium (engine)
// By DREADCRAFT
//

#include "base.h"

#include "GLFW/glfw3.h"

#include "defines.h"
#include "player.h"
#include "collision.h"

#include "../engine/umap.h"

/* Physics */
float verticalVelocity = 0.0f;
const float gravity = -0.005f;
float jumpForce = 0.15f;
bool isGrounded = false;

/* Input state */
bool jumpKeyPressed = false;
bool crouchKeyPressed = false;
bool cursorHidden = false;

/* Camera bob effect */
float cameraBobOffset = 0.0f;
float cameraBobTime = 0.0f;
const float cameraBobSpeed = 0.1f;
const float cameraBobAmount = 0.3f;
bool wasMoving = false;

/* Player collision bounds */
float playerWidth = 1.7f;
float playerHeight = 1.5f;
float playerHeightCrouch = 0.01f;
float playerDepth = 1.7f;

/* Mouse input */
float mouseSensitivity = 0.1f;
bool firstMouseInput = true;
double lastMouseX = 0.0;
double lastMouseY = 0.0;

/* Crouching */
bool isCrouching = false;
float targetCameraOffset = 0.0f;
float currentCameraOffset = 0.0f;
const float crouchCameraOffset = 2.5f;
const float cameraTransitionSpeed = 0.2f;

/* Player properties */
int health = 100;
int armor = 100;

/* Death state */
bool isPlayerDead = false;
float deathCameraOffset = 0.0f;
const float maxDeathCameraOffset = 3.0f;
const float deathCameraSpeed = 0.1f;

/* Camera (arrows) */
void handleArrowKeyCamera(void)
{
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
}

/* Camera bobbing */
static float calculateCameraBob(void)
{
    if (enableNoClip) return 0.0f;
    
    if (wasMoving && isGrounded) 
    {
        cameraBobTime += cameraBobSpeed;
        
        return sinf(cameraBobTime) * cameraBobAmount;
    }
    
    // Smoothly stop bobbing when movement stops
    if (cameraBobOffset != 0.0f) 
    {
        cameraBobOffset *= 0.9f;

        if (fabsf(cameraBobOffset) < 0.01f) 
        {
            cameraBobOffset = 0.0f;
        }
    }
    
    return cameraBobOffset;
}

/* Initialization */
void playerInit(void)
{
    verticalVelocity = 0.0f;
    isGrounded = false;
    isCrouching = false;
    jumpKeyPressed = false;
    crouchKeyPressed = false;
    enableNoClip = false;
    
    targetCameraOffset = 0.0f; 
    currentCameraOffset = 0.0f; 
    
    cameraBobOffset = 0.0f;
    cameraBobTime = 0.0f;
    wasMoving = false;
    firstMouseInput = true;

    health = 100;
    armor = 100;
    isPlayerDead = false;
    deathCameraOffset = 0.0f;

    // Center cursor only if focused
    if (focused) 
    {
        GLFWwindow* context = glfwGetCurrentContext();
        
        if (context) 
        {
            glfwSetCursorPos(context, screenWidth / 2, screenHeight / 2);
        }
    }
}

/* Handle player death */
void handlePlayerDeath(void)
{
    if (health <= 0 && !isPlayerDead)
    {
        isPlayerDead = true;
        verticalVelocity = 0.0f;
    }
}

/* Handle input when player is dead */
void handleDeathInput(void)
{
    if (!isPlayerDead) return;
    
    if (KEY_DOWN(INPUT_KEY_SPACE))
    {
        respawnPlayer();
    }
}

/* Respawn player */
void respawnPlayer(void)
{
    isPlayerDead = false;
    deathCameraOffset = 0.0f;
    health = 100;
    armor = 100;
    
    // Reset player position 
    playerX = 0.0f;
    playerY = 0.0f;
    playerZ = 5.0f;

    cameraX = 85.0f;
    cameraY = 0.0f;
    cameraZ = 0.0f;

    physicsEnabled = true;
}

/* Updating */
void playerUpdate(void)
{
    handlePlayerDeath();
    
    if (isPlayerDead)
    {
        if (deathCameraOffset < maxDeathCameraOffset)
        {
            deathCameraOffset += deathCameraSpeed;
            if (deathCameraOffset > maxDeathCameraOffset)
                deathCameraOffset = maxDeathCameraOffset;
        }
        
        float cameraZWithOffset = playerZ - currentCameraOffset - deathCameraOffset;
        
        glRotatef(-cameraX, 1, 0, 0);
        glRotatef(-cameraZ, 0, 0, 1);
        glTranslatef(-playerX, -playerY, -cameraZWithOffset);
        
        return;
    }

    if (enableNoClip) 
    {
        verticalVelocity = 0.0f;
        isGrounded = false;
    }
    else 
    {
        // Apply gravity if not grounded
        if (!isGrounded) 
        {
            verticalVelocity += gravity;
        } 
        else if (verticalVelocity < 0) 
        {
            verticalVelocity = 0;
        }

        // Apply vertical movement
        playerZ += verticalVelocity;

        // Check collisions
        checkPlayerCollisions(mapLoad);
        checkGrounded(mapLoad);
    }

    // Calculate camera bob effect
    float bobOffset = calculateCameraBob();

    // Calculate desired camera offset
    float desiredCameraOffset = currentCameraOffset;
    
    // Adjust camera position to avoid collision with brushes
    if (!enableNoClip) 
    {
        float playerPos[3] = {playerX, playerY, playerZ};
        adjustCameraPosition(playerPos, &desiredCameraOffset, mapLoad);
    }
    
    // Apply camera transformations with adjusted offset
    glRotatef(-cameraX + bobOffset, 1, 0, 0);
    glRotatef(-cameraZ, 0, 0, 1);
    
    float cameraZWithOffset = playerZ - desiredCameraOffset;
    glTranslatef(-playerX, -playerY, -cameraZWithOffset);
}

/* Position */
void setPlayerPosition(float x, float y, float z)
{
    physicsEnabled = false;
    playerX = x;
    playerY = y;
    playerZ = z;
    physicsEnabled = true;
    
    if (!enableNoClip && !isPlayerDead) 
    {
        checkPlayerCollisions(mapLoad);
        checkGrounded(mapLoad);
    }
}

/* Movement input */
void handleMovementInput(void)
{
    if (isPlayerDead) return;
    
    float angle = -cameraZ / 180 * PI;
    float speedX = 0.0f, speedY = 0.0f;
    
    const float baseSpeed = 0.1f;
    float currentSpeed = baseSpeed;

    if (enableNoClip) 
    {
        currentSpeed = baseSpeed;
    }

    // Slow movement mode
    else if (KEY_DOWN(INPUT_KEY_LEFT_SHIFT) || KEY_DOWN(INPUT_KEY_RIGHT_SHIFT) || isCrouching) 
    {
        currentSpeed *= 0.4f;
    }

    // Get movement input
    bool wPressed = KEY_DOWN(INPUT_KEY_W);
    bool sPressed = KEY_DOWN(INPUT_KEY_S);
    bool aPressed = KEY_DOWN(INPUT_KEY_A);
    bool dPressed = KEY_DOWN(INPUT_KEY_D);

    // Handle camera rotation with arrow keys
    handleArrowKeyCamera();

    // Handle crouching input 
    if (!enableNoClip) 
    {
        handleCrouchInput();
    }

    // Calculate movement direction
    if (wPressed && !sPressed) speedY += currentSpeed;
    if (sPressed && !wPressed) speedY -= currentSpeed;
    if (aPressed && !dPressed) speedX -= currentSpeed;
    if (dPressed && !aPressed) speedX += currentSpeed;

    // Update movement state for camera bob
    wasMoving = (speedX != 0 || speedY != 0);

    // Normalize diagonal movement
    if (speedX != 0 && speedY != 0) 
    {
        float length = sqrtf(speedX * speedX + speedY * speedY);
        speedX = speedX / length * currentSpeed;
        speedY = speedY / length * currentSpeed;
    }

    // Apply movement
    if (wasMoving) 
    {
        playerX += sinf(angle) * speedY + cosf(angle) * speedX;
        playerY += cosf(angle) * speedY - sinf(angle) * speedX;
    }
}

/* Jump input */
void handleJumpInput(void)
{
    if (isPlayerDead) return;
    
    if (enableNoClip) 
    {
        if (KEY_DOWN(INPUT_KEY_SPACE)) 
        {
            playerZ += 0.1f;
        }

        if (KEY_DOWN(INPUT_KEY_LEFT_CONTROL) && !isCrouching) 
        {
            playerZ -= 0.1f;
        }
        return;
    }
    
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

/* Crouch input */
void handleCrouchInput(void)
{
    if (isPlayerDead) return;
    if (enableNoClip) return;
    
    if (KEY_DOWN(INPUT_KEY_LEFT_CONTROL)) 
    {
        if (!crouchKeyPressed) 
        {
            isCrouching = true;
            playerHeight = playerHeightCrouch;
            targetCameraOffset = crouchCameraOffset;
            crouchKeyPressed = true;
        }
    } 
    else 
    {
        if (crouchKeyPressed) 
        {
            isCrouching = false;
            playerHeight = 1.5f;
            targetCameraOffset = 0.0f; 
            crouchKeyPressed = false;
        }
    }
    
    if (currentCameraOffset != targetCameraOffset) 
    {
        float difference = targetCameraOffset - currentCameraOffset;
        currentCameraOffset += difference * cameraTransitionSpeed;
        
        if (fabsf(difference) < 0.01f) 
        {
            currentCameraOffset = targetCameraOffset;
        }
    }
}

/* Camera input */
void handleMouseInput(GLFWwindow* window)
{
    if (!focused) 
    {
        if (cursorHidden) 
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            
            cursorHidden = false;
            firstMouseInput = true;
        }

        return;
    }

    if (!cursorHidden) 
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        cursorHidden = true;
        firstMouseInput = true;
    }
    
    inputSystemGetMousePosition(&mouseX, &mouseY);

    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;

    // Initialize mouse position on first call
    if (firstMouseInput) 
    {
        lastMouseX = centerX; 
        lastMouseY = centerY;
        firstMouseInput = false;
        
        glfwSetCursorPos(window, centerX, centerY);
        
        return;
    }

    // Calculate mouse movement
    int deltaX = (int)(mouseX - lastMouseX);
    int deltaY = (int)(mouseY - lastMouseY);
    
    // Apply camera rotation if mouse moved
    if (deltaX != 0 || deltaY != 0) 
    {
        cameraZ -= deltaX * mouseSensitivity;
        cameraX -= deltaY * mouseSensitivity;
        
        // Clamp vertical camera angle
        cameraX = fmaxf(0.0f, fminf(180.0f, cameraX));
            
        glfwSetCursorPos(window, centerX, centerY);
    }

    lastMouseX = centerX;
    lastMouseY = centerY;
}
