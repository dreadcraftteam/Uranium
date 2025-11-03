// Button control for GameUI
// By DREADCRAFT
// 

#include "base.h"
#include "label.h"
#include "button.h"

float buttonTextColor[4] = {1.0f, 1.0f, 1.0f, 1.0f}; 

/* Drawing buttons */
void drawButton(Button* button)
{
    if (!button) return;

    glPushAttrib(GL_CURRENT_BIT);

    glBegin(GL_QUADS);

        glColor3f(0.4f, 0.4f, 0.4f);

        glVertex2i(button->x, button->y);
        glVertex2i(button->x + button->width, button->y);
        glVertex2i(button->x + button->width, button->y + button->height);
        glVertex2i(button->x, button->y + button->height);

    glEnd();

    glBegin(GL_LINE_LOOP);

        if (button->isPressed) 
        {
            glColor3f(1.0f, 1.0f, 1.0f);
        } 
        else 
        {
            glColor3f(0.0f, 0.0f, 0.0f);
        }

        glVertex2i(button->x, button->y);
        glVertex2i(button->x + button->width, button->y);
        glVertex2i(button->x + button->width, button->y + button->height);
        glVertex2i(button->x, button->y + button->height);

    glEnd();

    glPopAttrib();

    if (button->text) 
    {
        int textWidth = getTextWidth(button->text);
        int textHeight = getTextHeight(button->text);
        int textX = button->x + (button->width - textWidth) / 2;
        int textY = button->y + (button->height - textHeight) / 2 + 2;

        drawString(textX, textY, button->text, buttonTextColor);
    }
}

/* Point in button */
int isPointInButton(Button* button, int x, int y)
{
    if (!button) return 0;
    return (x >= button->x && x <= button->x + button->width && 
            y >= button->y && y <= button->y + button->height);
}

/* State */
void buttonStateUpdate(Button* button, int mouseX, int mouseY, int mousePressed)
{
    if (!button) return;

    int wasHovered = button->isHovered;
    button->isHovered = isPointInButton(button, mouseX, mouseY);
    
    if (wasHovered != button->isHovered && button->hoverCallback) 
    {
        button->hoverCallback(button->isHovered);
    }

    if (mousePressed && button->isHovered) 
    {
        if (!button->isPressed) 
        {
            button->isPressed = 1;

            if (button->clickCallback) 
            {
                button->clickCallback();
            }
        }
    } 
    else 
    {
        button->isPressed = 0;
    }
}
