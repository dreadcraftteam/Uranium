// This file is needed to call the GameUI menus
// By DREADCRAFT
// 

#include "base.h"

// GameUI includes
#include "panel.h"
#include "imagepanel.h"
#include "gradientpanel.h"
#include "label.h"

// Hud
#include "gameui/hud_crosshair.h"
#include "gameui/hud_health.h"
#include "gameui/hud_armor.h"

int hud = 1;

/* Initialization */
void Game_GameUI_Init(void)
{
    if (hud == 1)
    {
        hudHealthInit(); 
        hudArmorInit();
        hudCrosshairInit();
    }
}

/* Updating */
void Game_GameUI_Update(GLFWwindow* frame)
{
    /* Handle button interactions */
    glfwGetCursorPos(frame, &mouseX, &mouseY);
    int mousePressed = (glfwGetMouseButton(frame, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);

    if (hud == 1)
    {
        drawHudHealth();
        drawHudArmor();
        drawHudCrosshair();
    }
}

/* Shutdown */
void Game_GameUI_Shutdown(void)
{
    
}
