// Drawing debug panel with options and debug information
// By DREADCRAFT
//

#ifndef DEBUGPANEL_H
#define DEBUGPANEL_H

#include "base.h"

// GameUI controls
#include "panel.h"
#include "label.h"
#include "button.h"

#include "variables.h"
#include "defines.h"

/* Callbacks */
static void buttonCollisionClickCallback(void);
static void buttonCollisionHoverCallback(int isHovered);
static void buttonCollisionWireframeClickCallback(void);
static void buttonCollisionWireframeHoverCallback(int isHovered);
static void buttonNoClipClickCallback(void);
static void buttonNoClipHoverCallback(int isHovered);
static void buttonQuitMapClickCallback(void);
static void buttonQuitMapHoverCallback(int isHovered);
static void buttonExitClickCallback(void);
static void buttonExitHoverCallback(int isHovered);
static void buttonKillPlayerClickCallback(void);
static void buttonKillPlayerHoverCallback(int isHovered);
static void buttonDamagePlayerClickCallback(void);
static void buttonDamagePlayerHoverCallback(int isHovered);
static void buttonHealPlayerClickCallback(void);
static void buttonHealPlayerHoverCallback(int isHovered);
static void buttonHudClickCallback(void);
static void buttonHudCallback(int isHovered);
static void buttonFPSCounterClickCallback(void);
static void buttonFPSCounterHoverCallback(int isHovered);

/* Debug panel initialization */
void debugPanelInit(void);

/* Debug panel updating */
void debugPanelUpdate(int mouseX, int mouseY, int mousePressed);

#endif // DEBUGPANEL_H
