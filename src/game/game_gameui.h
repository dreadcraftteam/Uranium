// This file is needed to call the GameUI menus
// By DREADCRAFT
// 

#ifndef GAME_GAMEUI_H
#define GAME_GAMEUI_H

#include "base.h"

// GameUI controls 
#include "panel.h"
#include "imagepanel.h"
#include "gradientpanel.h"
#include "label.h"

extern int hud;

/* Initialization */
void Game_GameUI_Init(void);

/* Updating */
void Game_GameUI_Update(GLFWwindow* frame);

/* Shutdown */
void Game_GameUI_Shutdown(void);

#endif // GAME_GAMEUI_H
