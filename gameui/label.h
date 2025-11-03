// Drawing and Rendering the Text
// By DREADCRAFT
//

#ifndef LABEL_H
#define LABEL_H

#include "base.h"

/* Initialize text rendering system */
void labelInit(void);

/* Draw a string at specified position */
void drawString(int x, int y, const char* text, float color[4]);

/* Draw a string using TrueType font */
void drawFontString(int x, int y, const char* text, const char* fontpath, float color[4], int fontSize);

/* Get text width in pixels */
int getTextWidth(const char* text);

/* Get text height in pixels */  
int getTextHeight(const char* text);

#endif // LABEL_H
