// Armor hud element for Player
// By DREADCRAFT
//

#ifndef HUD_ARMOR_H
#define HUD_ARMOR_H

#include "base.h"

// GameUI controls
#include "label.h"
#include "panel.h"

/* Panel */
extern float armorPanelColor[4];

/* Text */
extern float armorTextColor[4];
extern float armorTextColorColorCritical[4];

extern Panel panelArmor;

void hudArmorInit(void);

/* Update armor position based on screen size */
void updateArmorPosition(void);

/* Drawing hud element */
void drawHudArmor(void);

#endif // HUD_ARMOR_H
