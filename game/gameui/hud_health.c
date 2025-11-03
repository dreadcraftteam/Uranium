// Health hud element for Player
// By DREADCRAFT
//

#include "base.h"

// GameUI controls
#include "label.h"
#include "panel.h"

#include "hud_health.h"
#include "../player.h"
#include "../commands.h"

/* Panel */
float healthPanelColor[4] = {0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 60.0f / 255.0f};

/* Text */
float healthTextColor[4] = {255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f};
float healthTextColorCritical[4] = {255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f};

/* GameUI controls */
Panel panelHealth;

/* Health initialization */
void hudHealthInit(void)
{
    updateHealthPosition();
    
    panelHealth.width = 98;
    panelHealth.height = 75;

    for(int i = 0; i < 4; i++) 
    {
        panelHealth.color[i] = healthPanelColor[i];
    }

    panelHealth.outline = 0;
    panelHealth.isVisible = 1;
}

/* Update health position based on screen size */
void updateHealthPosition(void)
{
    int panelX = 10;
    int panelY = screenHeight - 85;

    panelHealth.x = panelX;
    panelHealth.y = panelY;
}

/* Drawing hud element */
void drawHudHealth(void)
{
    char healthText[16];
    float* textColor = healthTextColor;

    if(health <= 25)
    {
        textColor = healthTextColorCritical;
    }

    snprintf(healthText, sizeof(healthText), "%d", health);

    /* Update position in case screen size changed */
    updateHealthPosition();

    /* Drawing panel */
    drawPanel(&panelHealth);

    /* Text */
    drawFontString(panelHealth.x + 5, panelHealth.y + 5, "HEALTH", "font.ttf", textColor, 19);
    drawFontString(panelHealth.x + 5, panelHealth.y + 20, healthText, "font.ttf", textColor, 56);
}
