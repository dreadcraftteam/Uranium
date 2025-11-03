// Drawing debug panel with options and debug information
// By DREADCRAFT
//

#include "base.h"

// GameUI controls
#include "panel.h"
#include "label.h"
#include "button.h"

#include "console.h"
#include "variables.h"
#include "defines.h"
#include "debugpanel.h"

/* Colors */
float panelColor[4] = {130.0f / 255.0f, 130.0f / 255.0f, 130.0f / 255.0f, 255.0f / 255.0f};
float labelColor[4] = {255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f};

/* GameUI controls */
// Panel (for background)
static Panel panel;

// Dividers
static Panel divider1;
static Panel divider2;

// Buttons
static Button buttonCollision;
static Button buttonCollisionWireframe;
static Button buttonNoclip;
static Button buttonQuitMap;
static Button buttonExit;
static Button buttonKillPlayer;
static Button buttonDamagePlayer;
static Button buttonHealPlayer;
static Button buttonHud;
static Button buttonMatQuality;

/* State variable */
static bool buttonCollisionState = false;
static bool buttonCollisionWireframeState = false;
static bool buttonNoclipState = false;
static bool buttonQuitMapState = false;
static bool buttonExitState = false;
static bool buttonKillPlayerState = false;
static bool buttonDamagePlayerState = false;
static bool buttonHealPlayerState = false;
static bool buttonHudState = false;
static bool buttonMatQualityState = false;

/* Callbacks */
static void buttonCollisionClickCallback(void)
{
    buttonCollisionState = !buttonCollisionState;
    
    if (buttonCollisionState) 
    {
        buttonCollision.text = "Collision OFF";

        useCommand("collision 0");
    } 
    else 
    {
        buttonCollision.text = "Collision: ON";
        
        useCommand("collision 1");
    }
}

static void buttonCollisionHoverCallback(int isHovered)
{
    if (isHovered)
    {

    }
}

static void buttonCollisionWireframeClickCallback(void)
{
    buttonCollisionWireframeState = !buttonCollisionWireframeState;
    
    if (buttonCollisionWireframeState) 
    {
        buttonCollisionWireframe.text = "Collision Wireframe ON";

        useCommand("collision_wireframe 1");
    } 
    else 
    {
        buttonCollisionWireframe.text = "Collision Wireframe OFF";
        
        useCommand("collision_wireframe 0");
    }
}

static void buttonCollisionWireframeHoverCallback(int isHovered)
{
    if (isHovered)
    {

    }
}

static void buttonNoClipClickCallback(void)
{
    buttonNoclipState = !buttonNoclipState;
    
    if (buttonNoclipState) 
    {
        buttonNoclip.text = "NoClip ON";

        useCommand("noclip 1");
    } 
    else 
    {
        buttonNoclip.text = "NoClip OFF";
        
        useCommand("noclip 0");
    }
}

static void buttonNoClipHoverCallback(int isHovered)
{
    if (isHovered)
    {

    }
}

static void buttonQuitMapClickCallback(void)
{
    buttonQuitMapState = !buttonQuitMapState;
    
    if (buttonQuitMapState) 
    {
        useCommand("quit");
    }
}

static void buttonQuitMapHoverCallback(int isHovered)
{
    if (isHovered)
    {

    }
}

static void buttonExitClickCallback(void)
{
    buttonExitState = !buttonExitState;
    
    if (buttonExitState) 
    {
        useCommand("exit");
    }
}

static void buttonExitHoverCallback(int isHovered)
{
    if (isHovered)
    {

    }
}

static void buttonKillPlayerClickCallback(void)
{
    buttonKillPlayerState = !buttonKillPlayerState;
    
    if (buttonKillPlayerState) 
    {
        useCommand("kill");
    }
}

static void buttonKillPlayerHoverCallback(int isHovered)
{
    if (isHovered)
    {

    }
}

static void buttonDamagePlayerClickCallback(void)
{
    buttonDamagePlayerState = !buttonDamagePlayerState;
    
    if (buttonDamagePlayerState) 
    {
        useCommand("damage");
    }
}

static void buttonDamagePlayerHoverCallback(int isHovered)
{
    if (isHovered)
    {

    }
}

static void buttonHealPlayerClickCallback(void)
{
    buttonHealPlayerState = !buttonHealPlayerState;
    
    if (buttonHealPlayerState) 
    {
        useCommand("heal");
    }
}

static void buttonHealPlayerHoverCallback(int isHovered)
{
    if (isHovered)
    {

    }
}

static void buttonHudClickCallback(void)
{
    buttonHudState = !buttonHudState;
    
    if (buttonHudState) 
    {
        buttonHud.text = "Hud OFF";        
        
        useCommand("hud 0");
    }
    else
    {   
        buttonHud.text = "Hud ON";  

        useCommand("hud 1");
    }
}

static void buttonHudCallback(int isHovered)
{
    if (isHovered)
    {

    }
}

/* Debug panel initialization */
void debugPanelInit(void)
{
    // Panel (for background)
    panel.x = 25;
    panel.y = 25;
    panel.width = 500;
    panel.height = 250;
    panel.color[0] = panelColor[0]; // R
    panel.color[1] = panelColor[1]; // G
    panel.color[2] = panelColor[2]; // B
    panel.color[3] = panelColor[3];
    panel.outline = 1;
    panel.isVisible = 1;

    // Panel (for divider 1)
    divider1.x = 32;
    divider1.y = 32;
    divider1.width = 485;
    divider1.height = 40;
    divider1.outline = 1;
    divider1.isVisible = 1;

    // Panel (for divider 2)
    divider2.x = 32;
    divider2.y = 80;
    divider2.width = 485;
    divider2.height = 185;
    divider2.outline = 1;
    divider2.isVisible = 1;
    
    // Collision Button
    buttonCollision.x = 36;
    buttonCollision.y = 86;
    buttonCollision.width = 160;
    buttonCollision.height = 30;
    buttonCollision.text = "Collision: ON";
    buttonCollision.clickCallback = buttonCollisionClickCallback;
    buttonCollision.hoverCallback = buttonCollisionHoverCallback;
    buttonCollision.isHovered = 0;
    buttonCollision.isPressed = 0;
    
    // Collision Wireframe Button
    buttonCollisionWireframe.x = 36;
    buttonCollisionWireframe.y = 122;
    buttonCollisionWireframe.width = 160;
    buttonCollisionWireframe.height = 30;
    buttonCollisionWireframe.text = "Collision Wireframe OFF";
    buttonCollisionWireframe.clickCallback = buttonCollisionWireframeClickCallback;
    buttonCollisionWireframe.hoverCallback = buttonCollisionWireframeHoverCallback;
    buttonCollisionWireframe.isHovered = 0;
    buttonCollisionWireframe.isPressed = 0;

    // NoClip Button
    buttonNoclip.x = 36;
    buttonNoclip.y = 158;
    buttonNoclip.width = 160;
    buttonNoclip.height = 30;
    buttonNoclip.text = "NoClip OFF";
    buttonNoclip.clickCallback = buttonNoClipClickCallback;
    buttonNoclip.hoverCallback = buttonNoClipHoverCallback;
    buttonNoclip.isHovered = 0;
    buttonNoclip.isPressed = 0;

    // Quit Map Button
    buttonQuitMap.x = 36;
    buttonQuitMap.y = 194;
    buttonQuitMap.width = 160;
    buttonQuitMap.height = 30;
    buttonQuitMap.text = "Quit";
    buttonQuitMap.clickCallback = buttonQuitMapClickCallback;
    buttonQuitMap.hoverCallback = buttonQuitMapHoverCallback;
    buttonQuitMap.isHovered = 0;
    buttonQuitMap.isPressed = 0;

    // Exit Button
    buttonExit.x = 36;
    buttonExit.y = 230;
    buttonExit.width = 160;
    buttonExit.height = 30;
    buttonExit.text = "Exit";
    buttonExit.clickCallback = buttonExitClickCallback;
    buttonExit.hoverCallback = buttonExitHoverCallback;
    buttonExit.isHovered = 0;
    buttonExit.isPressed = 0;

    // Kill Player Button
    buttonKillPlayer.x = 203;
    buttonKillPlayer.y = 86;
    buttonKillPlayer.width = 160;
    buttonKillPlayer.height = 30;
    buttonKillPlayer.text = "Kill player";
    buttonKillPlayer.clickCallback = buttonKillPlayerClickCallback;
    buttonKillPlayer.hoverCallback = buttonKillPlayerHoverCallback;
    buttonKillPlayer.isHovered = 0;
    buttonKillPlayer.isPressed = 0;
    
    // Damage Player Button
    buttonDamagePlayer.x = 203;
    buttonDamagePlayer.y = 122;
    buttonDamagePlayer.width = 160;
    buttonDamagePlayer.height = 30;
    buttonDamagePlayer.text = "Damage player";
    buttonDamagePlayer.clickCallback = buttonDamagePlayerClickCallback;
    buttonDamagePlayer.hoverCallback = buttonDamagePlayerHoverCallback;
    buttonDamagePlayer.isHovered = 0;
    buttonDamagePlayer.isPressed = 0;

    // Heal Player Button
    buttonHealPlayer.x = 203;
    buttonHealPlayer.y = 158;
    buttonHealPlayer.width = 160;
    buttonHealPlayer.height = 30;
    buttonHealPlayer.text = "Heal player";
    buttonHealPlayer.clickCallback = buttonHealPlayerClickCallback;
    buttonHealPlayer.hoverCallback = buttonHealPlayerHoverCallback;
    buttonHealPlayer.isHovered = 0;
    buttonHealPlayer.isPressed = 0;

    // Hud Button
    buttonHud.x = 203;
    buttonHud.y = 194;
    buttonHud.width = 160;
    buttonHud.height = 30;
    buttonHud.text = "Hud ON";
    buttonHud.clickCallback = buttonHudClickCallback;
    buttonHud.hoverCallback = buttonHudCallback;
    buttonHud.isHovered = 0;
    buttonHud.isPressed = 0;
}

/* Debug panel updating */
void debugPanelUpdate(int mouseX, int mouseY, int mousePressed)
{
    // Button states
    buttonStateUpdate(&buttonCollision, mouseX, mouseY, mousePressed);
    buttonStateUpdate(&buttonCollisionWireframe, mouseX, mouseY, mousePressed);
    buttonStateUpdate(&buttonNoclip, mouseX, mouseY, mousePressed);
    buttonStateUpdate(&buttonQuitMap, mouseX, mouseY, mousePressed);
    buttonStateUpdate(&buttonExit, mouseX, mouseY, mousePressed);
    buttonStateUpdate(&buttonKillPlayer, mouseX, mouseY, mousePressed);
    buttonStateUpdate(&buttonDamagePlayer, mouseX, mouseY, mousePressed);
    buttonStateUpdate(&buttonHealPlayer, mouseX, mouseY, mousePressed);
    buttonStateUpdate(&buttonHud, mouseX, mouseY, mousePressed);

    // Panel (for background)
    drawPanel(&panel);

    // Dividers
    drawPanel(&divider1);
    drawPanel(&divider2);

    // Labels
    drawString(37, 42, "This is a debugging panel, but there are also some settings there. (A teaser of the", labelColor);
    drawString(37, 52, "settings menu?)", labelColor);

    // Buttons
    drawButton(&buttonCollision);
    drawButton(&buttonCollisionWireframe);
    drawButton(&buttonNoclip);
    drawButton(&buttonQuitMap);
    drawButton(&buttonExit);
    drawButton(&buttonKillPlayer);
    drawButton(&buttonDamagePlayer);
    drawButton(&buttonHealPlayer);
    drawButton(&buttonHud);
}
