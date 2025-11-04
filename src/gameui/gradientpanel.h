// Drawing panel with gradients
// By DREADCRAFT
//

#ifndef GRADIENTPANEL_H
#define GRADIENTPANEL_H

#include "base.h"

typedef struct 
{
    int x;
    int y;
    int width;
    int height;
    float firstColor[4];
    float secondColor[4];
    int outline;
    int isVisible;
} GradientPanel;

/* Drawing panel with Gradient */
void drawGradientPanel(GradientPanel* panel);

#endif // GRADIENTPANEL_H
