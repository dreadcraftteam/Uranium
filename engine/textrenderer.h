// Drawing and Rendering the Text
// By DREADCRAFT
//

#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include "base.h"

/* Initialize text rendering system */
void textRendererInit(void);

/* Set screen dimensions for orthographic projection */
void setScreenDimensions(int width, int height);

/* Begin text rendering (call before drawing any text) */
void beginTextRendering(void);

/* End text rendering (call after drawing all text) */
void endTextRendering(void);

/* Draw a string at specified position */
void drawString(const char* text, float x, float y);

/* Set text color (RGBA, 0-255) */
void setTextColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

/* Get text width in pixels */
int getTextWidth(const char* text);

/* Get text height in pixels */  
int getTextHeight(const char* text);

#endif // TEXTRENDERER_H

