// Console system and menu on GameUI
// By DREADCRAFT
//

#include "base.h"

#include "imagepanel.h"
#include "label.h"
#include "button.h"
#include "console.h"
#include "variables.h"
#include "commands.h"

#include "../game/commands.h"
#include "../materialsystem/commands.h"

static ImagePanel consoleBackground;

/* Console variable definition */
Console_t console = 
{
    .isOpen = 0,
    .targetHeight = 0,
    .currentHeight = 0,
    .animationSpeed = 7,
    .inputBuffer = {0},
    .cursorPos = 0,
    .blinkTimer = 0.0f,
    .cursorVisible = 1,
    .x = 0,
    .y = 0,
    .width = 800,
    .maxHeight = 600 / 2,
    .fullHeight = 600,
    .backgroundColor = {64.0f / 255.0f, 64.0f / 255.0f, 64.0f / 255.0f, 255.0f},
    .historyCount = 0,
    .historyPos = -1,
    .outputCount = 0,
    .isMapLoaded = 0,
    .forceOpen = 1,
    .defaultTextColor = {255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f},
    .errorTextColor = {255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f},
    .warningTextColor = {254.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f}
};

/* Initialize console system */
void consoleInit(void)
{
    console.width = screenWidth;
    console.fullHeight = screenHeight;
    console.x = 0;
    console.y = 0;
    
    console.isMapLoaded = 0;
    console.forceOpen = 1;
    
    consoleSetDimensions(0, 0, screenWidth, screenHeight);
}

/* Toggle console open/close */
void consoleToggle(void)
{
    if (console.forceOpen) 
    {
        return;
    }
    
    console.isOpen = !console.isOpen;
    console.targetHeight = console.isOpen ? console.maxHeight : 0;
    
    if (console.isOpen) 
    {
        memset(console.inputBuffer, 0, sizeof(console.inputBuffer));

        console.cursorPos = 0;
        console.cursorVisible = 1;

        console.blinkTimer = 0.0f;
        console.historyPos = -1;
    }
}

/* Update console animation */
void consoleUpdate(void)
{
    if (console.currentHeight < console.targetHeight) 
    {
        console.currentHeight += console.animationSpeed;
        
        if (console.currentHeight > console.targetHeight) 
        {
            console.currentHeight = console.targetHeight;
        }
    } 
    else if (console.currentHeight > console.targetHeight) 
    {
        console.currentHeight -= console.animationSpeed;
    
        if (console.currentHeight < console.targetHeight) 
        {
            console.currentHeight = console.targetHeight;
        }
    }
    
    if (console.isOpen) 
    {
        console.blinkTimer += 0.016f;
        
        if (console.blinkTimer >= 0.5f) 
        {
            console.cursorVisible = !console.cursorVisible;
            console.blinkTimer = 0.0f;
        }
    }
}

/* Draw the console */
void consoleDraw(void)
{
    if (console.currentHeight <= 0) return;
    
    glPushMatrix();
    
    float animationProgress = (float)console.currentHeight / (float)console.targetHeight;
    if (console.targetHeight == 0) animationProgress = 0.0f;
    
    int imageOffset = console.fullHeight - console.currentHeight;
    
    ImagePanel panel = {
        .x = console.x,
        .y = console.y - imageOffset,
        .width = console.width,
        .height = console.fullHeight,
        .imagePath = "ui/background.umf",
        .outline = 0,
        .isVisible = 1,
        .textureId = 0
    };
    
    drawImagePanel(&panel);
    
    glEnable(GL_SCISSOR_TEST);
    glScissor(console.x, screenHeight - (console.y + console.currentHeight), console.width, console.currentHeight);
    
    int inputHeight = 20;
    int inputY = console.y + console.currentHeight - inputHeight;
    
    drawString(console.x + 5, inputY + 5, "]", console.defaultTextColor);
    
    if (strlen(console.inputBuffer) > 0) 
    {
        drawString(console.x + 20, inputY + 5, console.inputBuffer, console.defaultTextColor);
    }
    
    if (console.cursorVisible && console.isOpen) 
    {
        int textWidth = getTextWidth(console.inputBuffer);
        int cursorX = console.x + 20 + textWidth;

        drawString(cursorX, inputY + 5, "_", console.defaultTextColor);
    }
    
    int lineHeight = 15;
    int availableHeight = console.currentHeight - inputHeight - 5;
    int maxLines = availableHeight / lineHeight;
    
    if (maxLines > 0) 
    {
        int startY = inputY - lineHeight;
        
        for (int i = 0; i < console.outputCount && i < maxLines; i++) 
        {
            int lineIndex = (console.outputCount - 1 - i) % 128;
            
            if (lineIndex < 0 || lineIndex >= 128) continue;
            
            float* textColor = console.defaultTextColor;
            const char* textToDraw = console.outputLines[lineIndex];
            
            if (textToDraw[0] == '^' && textToDraw[1] == '1') 
            {
                textColor = console.errorTextColor;
                textToDraw += 2;
            }
            else if (textToDraw[0] == '^' && textToDraw[1] == '2') 
            {
                textColor = console.warningTextColor;
                textToDraw += 2;
            }
            else if (textToDraw[0] == '^' && textToDraw[1] == '3') 
            {
                float customColor[4];
                const char* colorStart = textToDraw + 2;
                char* separator = strchr(colorStart, '|');
                
                if (separator != NULL) 
                {
                    char colorStr[64];
                    int colorLen = separator - colorStart;
                    if (colorLen < sizeof(colorStr)) 
                    {
                        strncpy(colorStr, colorStart, colorLen);
                        colorStr[colorLen] = '\0';
                        
                        if (sscanf(colorStr, "%f,%f,%f,%f", &customColor[0], &customColor[1], &customColor[2], &customColor[3]) == 4) 
                        {
                            textToDraw = separator + 1;
                            drawString(console.x + 5, startY - (i * lineHeight), textToDraw, customColor);

                            continue; 
                        }
                    }
                }
            }

            drawString(console.x + 5, startY - (i * lineHeight), textToDraw, textColor);
        }
    }
    
    glDisable(GL_SCISSOR_TEST);

    glPopMatrix();


#ifdef MUTATION
    // Draw version info in top right corner
    char versionText[64];
    snprintf(versionText, sizeof(versionText), "Uranium %s (Build %d)", version, build_number);
    
    int textWidth = getTextWidth(versionText);
    int versionX = console.x + console.width - textWidth - 4; // 10px padding from right edge
    int versionY = console.y + 5; // 10px padding from top
    
    drawString(versionX, versionY, versionText, console.defaultTextColor);
#endif
}

/* Handle keyboard input for console */
void consoleKeyInput(int key, int action)
{
    if (!console.isOpen) return;
    
    switch (key) 
    {
        case INPUT_KEY_ESCAPE:
            if (action == GLFW_PRESS) 
            {
                consoleToggle();
            }
            break;
            
        case INPUT_KEY_ENTER:
            if (action == GLFW_PRESS) 
            {
                consoleExecuteCommand();
            }
            break;
            
        case INPUT_KEY_BACKSPACE:
            if (action == GLFW_PRESS || action == GLFW_REPEAT) 
            {
                if (console.cursorPos > 0) 
                {
                    console.cursorPos--;
                    console.inputBuffer[console.cursorPos] = '\0';
                }
            }
            break;
            
        case INPUT_KEY_DOWN:
            if (action == GLFW_PRESS) 
            {
                if (console.historyPos > -1) 
                {
                    console.historyPos--;
                    if (console.historyPos >= 0) 
                    {
                        strcpy(console.inputBuffer, console.history[console.historyPos]);
                    } 
                    else 
                    {
                        memset(console.inputBuffer, 0, sizeof(console.inputBuffer));
                    }

                    console.cursorPos = strlen(console.inputBuffer);
                }
            }
            break;
            
        case INPUT_KEY_UP:
            if (action == GLFW_PRESS) 
            {
                if (console.historyCount > 0) 
                {
                    if (console.historyPos < console.historyCount - 1) 
                    {
                        console.historyPos++;

                        strcpy(console.inputBuffer, console.history[console.historyPos]);

                        console.cursorPos = strlen(console.inputBuffer);
                    }
                }
            }
            break;
    }
}

/* Handle character input for console */
void consoleCharInput(unsigned int codepoint)
{
    if (!console.isOpen) return;
    
    if (codepoint == 32 || (codepoint >= 33 && codepoint <= 126)) 
    {
        if (console.cursorPos < sizeof(console.inputBuffer) - 1) 
        {
            console.inputBuffer[console.cursorPos] = (char)codepoint;
            console.cursorPos++;

            console.inputBuffer[console.cursorPos] = '\0';
        }
    }
}

/* Console key callbacks */
void consoleKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (consoleIsOpen()) 
    {
        consoleKeyInput(key, action);
    }
}

/* Console char callbacks */
void consoleCharCallback(GLFWwindow* window, unsigned int codepoint)
{
    if (consoleIsOpen()) 
    {
        consoleCharInput(codepoint);
    }
}

/* Add output line to console */
void consoleAddOutput(const char* text)
{
    /* NOTE: Do not use consoleAddOutput(), it is better to use Msg(), Warning() and Error()! */

    if (console.outputCount < 128)
    {
        strncpy(console.outputLines[console.outputCount], text, 255);
        
        console.outputCount++;
    } 
    else 
    {
        for (int i = 0; i < 127; i++)
        {
            strcpy(console.outputLines[i], console.outputLines[i + 1]);
        }

        strncpy(console.outputLines[127], text, 255);
    }
}

/* Set map loaded state */
void consoleSetMapLoaded(int loaded)
{
    console.isMapLoaded = loaded;
    console.forceOpen = !loaded;
    
    consoleSetDimensions(console.x, console.y, console.width, console.fullHeight);
    
    if (loaded) 
    {
        if (console.isOpen) 
        {
            console.animationSpeed = 7;
            consoleToggle(); 
        }
    }
    else 
    {
        console.isOpen = 1;
        console.targetHeight = console.fullHeight;
        console.forceOpen = 1;
    }
}

/* Execute config files */
void consoleExecuteConfigFile(const char* filename)
{
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "config/%s", filename);

    FILE* file = fopen(filepath, "r");
    if (!file)
    {
        Error("Could not open config file: %s\n", filepath);
        return;
    }

    Msg("Executing: %s\n", filepath);

    char line[256];
    int lineNum = 0;

    while (fgets(line, sizeof(line), file))
    {
        lineNum++;

        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n')
            line[len-1] = '\0';

        if (strlen(line) == 0 || line[0] == '#' || line[0] == '/')
            continue;

        int commandFound = consoleCommands(line);
        
        if (!commandFound) 
        {
            commandFound = gameConsoleCommands(line);
        }

        if (!commandFound) 
        {
            commandFound = materialsystemConsoleCommands(line);
        }
        
        if (!commandFound) 
        {
            Msg("Unknown command: %s\n", line);
        }
    }

    fclose(file);
}

/* Execute console command */
void consoleExecuteCommand(void)
{
    if (strlen(console.inputBuffer) == 0) return;
    
    char commandOutput[256];
    snprintf(commandOutput, sizeof(commandOutput), "] %s", console.inputBuffer);
    Msg("%s\n", commandOutput);
    
    int commandFound = 0;
    
    commandFound = consoleCommands(console.inputBuffer);
    
    if (!commandFound) 
    {
        commandFound = gameConsoleCommands(console.inputBuffer);
    }

    if (!commandFound) 
    {
        commandFound = materialsystemConsoleCommands(console.inputBuffer);
    }
    
    if (!commandFound) 
    {
        Msg("Unknown command: %s\n", console.inputBuffer);
    }
    
    // Add to command history
    if (console.historyCount < 128)
    {
        strcpy(console.history[console.historyCount], console.inputBuffer);
        console.historyCount++;
    }
    else
    {
        for (int i = 0; i < 127; i++)
        {
            strcpy(console.history[i], console.history[i + 1]);
        }
        
        strcpy(console.history[127], console.inputBuffer);
    }
    
    console.historyPos = -1;
    memset(console.inputBuffer, 0, sizeof(console.inputBuffer));
    console.cursorPos = 0;
}

/* Check if console is open */
int consoleIsOpen(void)
{
    return console.isOpen;
}

/* Set console dimensions */
void consoleSetDimensions(int x, int y, int width, int height)
{
    console.x = x;
    console.y = y;
    console.width = width;
    console.fullHeight = height;
    
    if (console.forceOpen && !console.isMapLoaded) 
    {
        console.maxHeight = console.fullHeight;
        console.targetHeight = console.fullHeight;
        console.isOpen = 1;
        console.animationSpeed = 7;
    }
    else 
    {
        console.maxHeight = console.fullHeight / 2;
        if (console.isOpen) 
        {
            console.targetHeight = console.maxHeight;
        }

        console.animationSpeed = 7;
    }
    
    console.currentHeight = console.targetHeight;
}

/* Execute console commands from code */
void useCommand(const char* command)
{
    if (!command || strlen(command) == 0) return;
    
    char commandOutput[256];
    snprintf(commandOutput, sizeof(commandOutput), "] %s", command);
    Msg("%s\n", commandOutput);
    
    int commandFound = 0;
    
    commandFound = consoleCommands(command);
    
    if (!commandFound) 
    {
        commandFound = gameConsoleCommands(command);
    }

    if (!commandFound) 
    {
        commandFound = materialsystemConsoleCommands(command);
    }
    
    if (!commandFound) 
    {
        Msg("Unknown command: %s\n", command);
    }
}
