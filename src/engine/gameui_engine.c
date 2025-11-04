// This file is needed to call the GameUI menus
// By DREADCRAFT
// 

#include "base.h"

// GameUI includes 
#include "panel.h"
#include "imagepanel.h"
#include "gradientpanel.h"
#include "label.h"
#include "button.h"

#include "debugpanel.h"
#include "gameui_engine.h"

/* Initialization */
void Engine_GameUI_Init(void)
{
    debugPanelInit();
}

/* Updating */
void Engine_GameUI_Update(GLFWwindow* frame)
{
    /* Handle button interactions */
    glfwGetCursorPos(frame, &mouseX, &mouseY);
    int mousePressed = (glfwGetMouseButton(frame, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);

    if (enableDebugPanel == 1)
    {
        focused = false;
    }

    if (enableDebugPanel == 1)
    {
        debugPanelUpdate((int)mouseX, (int)mouseY, mousePressed);
    }
}

/* Shutdown */
void Engine_GameUI_Shutdown(void)
{
    
}
