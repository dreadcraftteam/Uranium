// Console system and menu on GameUI
// By DREADCRAFT
//

#ifndef CONSOLE_H
#define CONSOLE_H

#include "base.h"

#include "imagepanel.h"
#include "label.h"

/* Console states */
typedef struct Console_s
{
    int isOpen;
    int targetHeight;
    int currentHeight;
    int animationSpeed;
    char inputBuffer[256];
    int cursorPos;
    float blinkTimer;
    int cursorVisible;
    int x, y, width;
    int maxHeight;
    int fullHeight;
    float backgroundColor[4];
    char history[128][256];
    int historyCount;
    int historyPos;
    char outputLines[128][256];
    int outputCount;
    int isMapLoaded;
    int forceOpen;
    float defaultTextColor[4];
    float errorTextColor[4];
    float warningTextColor[4];
} Console_t;

/* External console variable */
extern Console_t console;

/* Initialize console system */
void consoleInit(void);

/* Toggle console open/close */
void consoleToggle(void);

/* Update console animation */
void consoleUpdate(void);

/* Draw the console */
void consoleDraw(void);

/* Handle keyboard input for console */
void consoleKeyInput(int key, int action);

/* Handle character input for console */
void consoleCharInput(unsigned int codepoint);

/* Console key callbacks */
void consoleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

/* Console char callbacks */
void consoleCharCallback(GLFWwindow* window, unsigned int codepoint);

/* Add output line to console */
void consoleAddOutput(const char* text);

/* Execute config files */
void consoleExecuteConfigFile(const char* filename);

/* Execute console command */
void consoleExecuteCommand(void);

/* Check if console is open */
int consoleIsOpen(void);

/* Set map loaded state */
void consoleSetMapLoaded(int loaded);

/* Set console dimensions */
void consoleSetDimensions(int x, int y, int width, int height);

/* Execute console commands from code */
void useCommand(const char* command);

#endif // CONSOLE_H