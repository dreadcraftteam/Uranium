// Crosshair on GameUI
// By DREADCRAFT
//

#ifndef CROSSHAIR_H
#define CROSSHAIR_H

#include "base.h"

// GameUI controls
#include "panel.h"

#include "../engine/variables.h"

extern float crosshairColor[4];

extern Panel verticalBar;
extern Panel horizontalBar;

/* Crosshair initialization */
void hudCrosshairInit(void);

/* Update crosshair position based on screen size */
void updateCrosshairPosition(void);

/* Drawing crosshair */
void drawHudCrosshair(void);

#endif // CROSSHAIR_H
