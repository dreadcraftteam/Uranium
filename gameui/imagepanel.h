// Drawing panel with image
// By DREADCRAFT
//

#ifndef IMAGEPANEL_H
#define IMAGEPANEL_H

#include "base.h"

typedef struct 
{
    int x;
    int y;
    int width;
    int height;
    const char* imagePath;
    int outline;
    int isVisible;
    GLuint textureId; // cached texture
} ImagePanel;

/* Drawing panel with image */
void drawImagePanel(ImagePanel* panel);

#endif // IMAGEPANEL_H
