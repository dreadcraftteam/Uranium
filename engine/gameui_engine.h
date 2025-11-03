// This file is needed to call the GameUI menus
// By DREADCRAFT
// 

#ifndef GAMEUI_ENGINE_H
#define GAMEUI_ENGINE_H

#include "base.h"

// GameUI includes 
#include "panel.h"
#include "imagepanel.h"
#include "gradientpanel.h"
#include "label.h"
#include "button.h"

// For debug panel :)
extern bool previousFocusedState;

/* Initialization */
void Engine_GameUI_Init(void);

/* Updating */
void Engine_GameUI_Update(GLFWwindow* frame);

/* Shutdown */
void Engine_GameUI_Shutdown(void);

#endif // GAMEUI_ENGINE_H
