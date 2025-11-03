// Drawing and Rendering the Text
// By DREADCRAFT
//

#include "base.h"

#include "stb/stb_easy_font.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"

#include "label.h"

static char textBuffer[60000];
float textColor[4] = {255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f};

/* Initialize text rendering system */
void labelInit(void)
{
    memset(textBuffer, 0, sizeof(textBuffer));
}

/* Draw a string at specified position */
void drawString(int x, int y, const char* text, float color[4])
{
    if (!text || strlen(text) == 0) return;

    // Convert float color [0-1] to unsigned char [0-255]
    unsigned char currentColor[4];
    if (color)
    {
        currentColor[0] = (unsigned char)(color[0] * 255);
        currentColor[1] = (unsigned char)(color[1] * 255);
        currentColor[2] = (unsigned char)(color[2] * 255);
        currentColor[3] = (unsigned char)(color[3] * 255);
    }
    else
    {
        // Use default white color if no color provided
        memcpy(currentColor, textColor, sizeof(textColor));
    }

    int num_quads = stb_easy_font_print(x, y, (char*)text, currentColor, textBuffer, sizeof(textBuffer));

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

/* Draw a string using TrueType font */
void drawFontString(int x, int y, const char* text, const char* fontpath, float color[4], int fontSize)
{
    if (!text || strlen(text) == 0) return;

    char fullPath[512];

    if (fontpath && strlen(fontpath) > 0)
        snprintf(fullPath, sizeof(fullPath), "fonts/%s", fontpath);

    FILE* fontFile = fopen(fullPath, "rb");
    if (!fontFile)
    {
        Error("Could not open font file: %s\n", fullPath);

        return;
    }

    fseek(fontFile, 0, SEEK_END);

    size_t size = ftell(fontFile);

    fseek(fontFile, 0, SEEK_SET);

    unsigned char* fontBuffer = (unsigned char*)malloc(size);

    fread(fontBuffer, 1, size, fontFile);
    fclose(fontFile);

    stbtt_fontinfo font;

    if (!stbtt_InitFont(&font, fontBuffer, stbtt_GetFontOffsetForIndex(fontBuffer, 0)))
    {
        Error("Failed to init font from: %s\n", fullPath);
        free(fontBuffer);

        return;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLboolean lightingEnabled = glIsEnabled(GL_LIGHTING);

    if (lightingEnabled) glDisable(GL_LIGHTING);

    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_LIGHTING_BIT | GL_COLOR_BUFFER_BIT);

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (color)
        glColor4f(color[0], color[1], color[2], color[3]);
    else
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    if (fontSize <= 0) fontSize = 24;
    float scale = stbtt_ScaleForPixelHeight(&font, (float)fontSize);

    int ascent, descent, lineGap;
    stbtt_GetFontVMetrics(&font, &ascent, &descent, &lineGap);
    float baseline = ascent * scale;

    float xpos = (float)x;
    const char* p = text;

    while (*p)
    {
        int advance, lsb, w, h, xoff, yoff;
        stbtt_GetCodepointHMetrics(&font, *p, &advance, &lsb);
        unsigned char* bitmap = stbtt_GetCodepointBitmap(&font, 0, scale, *p, &w, &h, &xoff, &yoff);

        unsigned char* rgba = malloc(w * h * 4);
        for (int j = 0; j < h; j++)
            for (int i = 0; i < w; i++) 
            {
                int idx = j * w + i;
                rgba[idx * 4 + 0] = (unsigned char)((color ? color[0] : 1.0f) * 255);
                rgba[idx * 4 + 1] = (unsigned char)((color ? color[1] : 1.0f) * 255);
                rgba[idx * 4 + 2] = (unsigned char)((color ? color[2] : 1.0f) * 255);
                rgba[idx * 4 + 3] = bitmap[idx];
            }

        float yPos = y + baseline + yoff;

        GLuint tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);

        glEnable(GL_TEXTURE_2D);

        glBegin(GL_QUADS);

        glTexCoord2f(0, 0); glVertex2f(xpos + xoff,     yPos);
        glTexCoord2f(1, 0); glVertex2f(xpos + xoff + w, yPos);
        glTexCoord2f(1, 1); glVertex2f(xpos + xoff + w, yPos + h);
        glTexCoord2f(0, 1); glVertex2f(xpos + xoff,     yPos + h);

        glEnd();

        glDisable(GL_TEXTURE_2D);
        glDeleteTextures(1, &tex);

        free(rgba);
        stbtt_FreeBitmap(bitmap, NULL);

        xpos += advance * scale;
        if (*(p + 1))
            xpos += scale * stbtt_GetCodepointKernAdvance(&font, *p, *(p + 1));

        p++;
    }

    if (lightingEnabled) glEnable(GL_LIGHTING);

    glDisable(GL_BLEND);
    free(fontBuffer);

    glPopAttrib();
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
