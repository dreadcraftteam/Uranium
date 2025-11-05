// FPS counter for debugging
// By DREADCRAFT
//

#include "base.h"

#include "GLFW/glfw3.h"
#include "label.h"

#include "fps.h"

// Variables for FPS counter
static double previousTime = 0.0;
static int frameCount = 0;
static double currentFPS = 0.0;
static char fpsText[32] = "FPS: 0";

/* Initialization */
void fpsCounterInit(void)
{
    previousTime = glfwGetTime();
    frameCount = 0;
    currentFPS = 0.0;
    strcpy(fpsText, "FPS: 0");
}

/* Updating */
void fpsCounterUpdate(void)
{
    double currentTime = glfwGetTime();
    frameCount++;

    // Update FPS every second
    if (currentTime - previousTime >= 1.0)
    {
        currentFPS = (double)frameCount / (currentTime - previousTime);
        frameCount = 0;
        previousTime = currentTime;

        // Format FPS text as integer
        snprintf(fpsText, sizeof(fpsText), "FPS: %d", (int)currentFPS);
    
        Msg("FPS: %d\n", (int)currentFPS);
    }

    float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

    drawString(10, 10, fpsText, color);
}
