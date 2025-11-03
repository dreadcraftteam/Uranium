// Health hud element for Player
// By DREADCRAFT
//

#ifndef HUD_HEALTH_H
#define HUD_HEALTH_H

#include "base.h"

// GameUI controls
#include "label.h"
#include "panel.h"

/* Panel */
extern float healthPanelColor[4];

/* Text */
extern float healthTextColor[4];
extern float healthTextColorCritical[4];

extern Panel panelHealth;

/* Health initialization */
void hudHealthInit(void);

/* Update health position based on screen size */
void updateHealthPosition(void);

/* Drawing hud element */
void drawHudHealth(void);

#endif // HUD_HEALTH_H
