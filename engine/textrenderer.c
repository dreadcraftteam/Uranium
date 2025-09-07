// Drawing and Rendering the Text
// By DREADCRAFT
//

#include "base.h"

#include "stb/stb_easy_font.h"
#include "textrenderer.h"

static char textBuffer[60000];
static unsigned char textColor[4] = {255, 255, 255, 255};

/* Screen dimensions */
static int screen_width = 800;
static int screen_height = 600;

/* Initialize text rendering system */
void textRendererInit(void)
{
    memset(textBuffer, 0, sizeof(textBuffer));
    
    Msg("Text renderer initialized\n");
}

/* Set screen dimensions for orthographic projection */
void setScreenDimensions(int width, int height)
{
    screen_width = width;
    screen_height = height;
}

/* Begin text rendering (call before drawing any text) */
void beginTextRendering(void)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, screen_width, screen_height, 0, -1, 1);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/* End text rendering (call after drawing all text) */
void endTextRendering(void)
{
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

/* Draw a string at specified position */
void drawString(const char* text, float x, float y)
{
    if (!text || strlen(text) == 0) return;

    int num_quads = stb_easy_font_print(x, y, (char*)text, textColor, textBuffer, sizeof(textBuffer));

    if (num_quads > 0)
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        
        glVertexPointer(2, GL_FLOAT, 16, textBuffer);
        glColorPointer(4, GL_UNSIGNED_BYTE, 16, textBuffer + 12);
        
        glDrawArrays(GL_QUADS, 0, num_quads * 4);
        
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
    }
}

/* Set text color (RGBA, 0-255) */
void setTextColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    textColor[0] = r;
    textColor[1] = g;
    textColor[2] = b;
    textColor[3] = a;
}

/* Get text width in pixels */
int getTextWidth(const char* text)
{
    if (!text) return 0;
    return stb_easy_font_width((char*)text);
}

/* Get text height in pixels */
int getTextHeight(const char* text)
{
    if (!text) return 0;
    return stb_easy_font_height((char*)text);
}

