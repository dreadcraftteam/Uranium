// Crosshair on GameUI
// By DREADCRAFT
//

#include "base.h"

// GameUI controls
#include "panel.h"

#include "hud_crosshair.h"

float crosshairColor[4] = {255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f};

/* GameUI controls */
Panel verticalBar;
Panel horizontalBar;

/* Crosshair initialization */
void hudCrosshairInit(void)
{
    updateCrosshairPosition();
    
    int size = 8;
    int thickness = 1;

    verticalBar.width = thickness;
    verticalBar.height = size * 2 + 1;

    for(int i = 0; i < 4; i++) 
    {
        verticalBar.color[i] = crosshairColor[i];
    }

    verticalBar.outline = 0;
    verticalBar.isVisible = 1;

    horizontalBar.width = size * 2 + 1;
    horizontalBar.height = thickness;

    for(int i = 0; i < 4; i++) 
    {
        horizontalBar.color[i] = crosshairColor[i];
    }

    horizontalBar.outline = 0;
    horizontalBar.isVisible = 1;
}

/* Update crosshair position based on screen size */
void updateCrosshairPosition(void)
{
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;
    int size = 8;

    // Vertical bar
    verticalBar.x = centerX;
    verticalBar.y = centerY - size;

    // Horizontal bar  
    horizontalBar.x = centerX - size;
    horizontalBar.y = centerY;
}

/* Drawing crosshair */
void drawHudCrosshair(void)
{
    /* Update position in case screen size changed */
    updateCrosshairPosition();

    drawPanel(&verticalBar);
    drawPanel(&horizontalBar);
}
