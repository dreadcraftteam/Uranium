// GameUI pause menu
// By DREADCRAFT
//

#include "base.h"

#include "GLFW/glfw3.h"

#include "../variables.h"
#include "pause.h"

#ifdef USE_GAMEUI
static bool isPaused = false;
static int selectedOption = 0;
static const char* menuOptions[] = {"Resume", "Settings", "Exit to Menu", "Quit Game"};
static const int menuOptionsCount = 4;

static float menuBackgroundColor[4] = {0.1f, 0.1f, 0.1f, 0.3f};
static float menuPanelColor[4] = {0.2f, 0.2f, 0.2f, 0.8f}; 
static float menuTextColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
static float menuSelectedColor[4] = {0.2f, 0.6f, 1.0f, 1.0f};

static double lastInputTime = 0.0;

void drawRect(float x, float y, float width, float height, float color[4]) 
{
    glColor4fv(color);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void drawRoundedRect(float x, float y, float width, float height, float color[4], float radius) 
{
    glColor4fv(color);
    
    glBegin(GL_QUADS);
    glVertex2f(x + radius, y);
    glVertex2f(x + width - radius, y);
    glVertex2f(x + width - radius, y + height);
    glVertex2f(x + radius, y + height);
    glEnd();
    
    glBegin(GL_QUADS);
    glVertex2f(x, y + radius);
    glVertex2f(x + radius, y + radius);
    glVertex2f(x + radius, y + height - radius);
    glVertex2f(x, y + height - radius);
    glEnd();
    
    glBegin(GL_QUADS);
    glVertex2f(x + width - radius, y + radius);
    glVertex2f(x + width, y + radius);
    glVertex2f(x + width, y + height - radius);
    glVertex2f(x + width - radius, y + height - radius);
    glEnd();
    
    glBegin(GL_TRIANGLE_FAN);

    for (int i = 0; i <= 10; i++) 
    {
        float angle = M_PI * 1.5f + (float)i * (M_PI / 20.0f);
        glVertex2f(x + radius + cos(angle) * radius, y + radius + sin(angle) * radius);
    }
    glEnd();
}

void renderPauseMenu(GLFWwindow* window) 
{
    if (!isPaused) return;

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    GLboolean depthTest = glIsEnabled(GL_DEPTH_TEST);
    GLboolean lighting = glIsEnabled(GL_LIGHTING);
    GLboolean texture2D = glIsEnabled(GL_TEXTURE_2D);
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    drawRect(0, 0, width, height, menuBackgroundColor);

    float panelWidth = 300.0f;
    float panelHeight = 250.0f;
    float panelX = (width - panelWidth) / 2;
    float panelY = (height - panelHeight) / 2;
    
    drawRect(panelX, panelY, panelWidth, panelHeight, menuPanelColor);

    float optionSpacing = 40.0f;
    float startY = panelY + 70.0f;
    
    for (int i = 0; i < menuOptionsCount; i++) 
    {
        float y = startY + i * optionSpacing;
        float* color = (i == selectedOption) ? menuSelectedColor : menuTextColor;
        
        if (i == selectedOption) 
        {
            float highlightColor[4] = {0.3f, 0.3f, 0.3f, 0.5f};
            drawRect(panelX + 20, y - 5, panelWidth - 40, 30, highlightColor);
        }
        
        drawRect(panelX + panelWidth / 2 - 60, y, 120, 20, color);
    }

    drawRect(panelX + panelWidth / 2 - 40, panelY + 20, 80, 25, menuTextColor);

    if (depthTest) glEnable(GL_DEPTH_TEST);
    if (lighting) glEnable(GL_LIGHTING);
    if (texture2D) glEnable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    //glMatrixMode(GL_PROJECTION);
    //glPopMatrix();
    //glMatrixMode(GL_MODELVIEW);
    //glPopMatrix();
}

void handlePauseMenuInput(GLFWwindow* window) 
{
    if (!isPaused) return;

    double currentTime = glfwGetTime();
    
    if (currentTime - lastInputTime < 0.2) return;

    if (KEY_PRESSED(INPUT_KEY_UP)) 
    {
        selectedOption = (selectedOption - 1 + menuOptionsCount) % menuOptionsCount;
        lastInputTime = currentTime;
        Msg("Selected option: %d\n", selectedOption);
    }
    
    if (KEY_PRESSED(INPUT_KEY_DOWN)) 
    {
        selectedOption = (selectedOption + 1) % menuOptionsCount;
        lastInputTime = currentTime;
        Msg("Selected option: %d\n", selectedOption);
    }

    if (KEY_PRESSED(INPUT_KEY_ENTER) || KEY_PRESSED(INPUT_KEY_SPACE)) 
    {
        lastInputTime = currentTime;
        
        switch (selectedOption) 
        {
            case 0: // Resume
                togglePause();
                break;
            case 1: // Settings
                Msg("Settings menu selected\n");
                break;
            case 2: // Exit to Menu
                Msg("Exit to menu selected\n");
                running = false;
                break;
            case 3: // Quit Game
                Msg("Quit game selected\n");
                running = false;
                break;
        }
    }

    if (KEY_PRESSED(INPUT_KEY_ESCAPE)) 
    {
        lastInputTime = currentTime;
        togglePause();
    }
}

void togglePause() 
{
    isPaused = !isPaused;
    
    if (isPaused) 
    {
        Msg("Game paused\n");
        selectedOption = 0; 
    }
    else
    {
        Msg("Game resumed\n");
    }
}

bool isGamePaused() 
{
    return isPaused;
}

void updatePauseMenu(GLFWwindow* window) 
{
    if (KEY_PRESSED(INPUT_KEY_ESCAPE) && !isPaused) 
    {
        togglePause();
        return;
    }

    if (isPaused) 
    {
        handlePauseMenuInput(window);
    }
}
#endif
