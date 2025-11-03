// Drawing normal panel (with colors)
// By DREADCRAFT
//

#ifndef PANEL_H
#define PANEL_H

#include "base.h"

typedef struct 
{
    int x;
    int y;
    int width;
    int height;
    float color[4];
    int outline;
    int isVisible;
} Panel;

/* Drawing basic panel */
void drawPanel(Panel* panel);

#endif // PANEL_H
