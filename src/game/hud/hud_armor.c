// Armor hud element for Player
// By DREADCRAFT
//

#include "base.h"

// GameUI controls
#include "label.h"
#include "panel.h"

#include "hud_armor.h"
#include "../player.h"
#include "../commands.h"

/* Panel */
float armorPanelColor[4] = {0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 60.0f / 255.0f};

/* Text */
float armorTextColor[4] = {255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f};
float armorTextColorCritical[4] = {255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f};

/* GameUI controls */
Panel panelArmor;

/* Armor initialization */
void hudArmorInit(void)
{
    updateArmorPosition();
    
    panelArmor.width = 98;
    panelArmor.height = 75;
    
    for(int i = 0; i < 4; i++) 
    {
        panelArmor.color[i] = armorPanelColor[i];
    }
    
    panelArmor.outline = 0;
    panelArmor.isVisible = 1;
}

/* Update armor position based on screen size */
void updateArmorPosition(void)
{
    int panelX = 10 + 98 + 10;
    int panelY = screenHeight - 85;

    panelArmor.x = panelX;
    panelArmor.y = panelY;
}

/* Drawing hud element */
void drawHudArmor(void)
{
    char armorText[16];
    float* textColor = armorTextColor;

    if(armor <= 25)
    {
        textColor = armorTextColorCritical;
    }

    snprintf(armorText, sizeof(armorText), "%d", armor);

    /* Update position in case screen size changed */
    updateArmorPosition();

    /* Drawing panel */
    drawPanel(&panelArmor);

    /* Text */
    drawFontString(panelArmor.x + 5, panelArmor.y + 5, "ARMOR", "font.ttf", textColor, 19);
    drawFontString(panelArmor.x + 5, panelArmor.y + 20, armorText, "font.ttf", textColor, 56);
}
