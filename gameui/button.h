// Button control for GameUI
// By DREADCRAFT
// 

#ifndef BUTTON_H
#define BUTTON_H

typedef void (*ButtonClickCallback)(void);
typedef void (*ButtonHoverCallback)(int isHovered);

extern float buttonTextColor[4];

typedef struct 
{
    int x;
    int y;
    int width;
    int height;
    const char* text;
    ButtonClickCallback clickCallback;
    ButtonHoverCallback hoverCallback;
    int isHovered;
    int isPressed;
} Button;

/* Drawing buttons */
void drawButton(Button* button);

/* Point in button */
int isPointInButton(Button* button, int x, int y);

/* State */
void buttonStateUpdate(Button* button, int mouseX, int mouseY, int mousePressed);

#endif
