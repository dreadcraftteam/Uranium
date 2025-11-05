// FPS counter for debugging
// By DREADCRAFT
//

// funfact: this is a test fps counter from build with UFM.

#include "base.h"

#include "GLFW/glfw3.h"
#include "label.h"

#include "fps.h"

static int fps = 0;
static double lastFpsUpdateTime = 0.0;
double lastTime;
int frameCount;

/* Initialization */
void fpsCounterInit(void)
{
    lastTime = glfwGetTime();
    frameCount = 0;
}

/* Updating */
void fpsCounterUpdate(void)
{
    frameCount++;
    double currentTime = glfwGetTime();

    if (currentTime - lastTime >= 1.0) 
    {
        Msg("FPS: %d\n", frameCount);
            
        // Updating FPS on screen...
        fps = frameCount;
            
        frameCount = 0;
        lastTime = currentTime;
    }

    char fpsText[32];
    snprintf(fpsText, sizeof(fpsText), "FPS: %d", fps);

    float FPSCounterColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};

    drawString(10, 10, fpsText, FPSCounterColor);
}
