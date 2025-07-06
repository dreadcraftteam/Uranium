// Basic Text Rendering for Uranium Engine
// By DREADCRAFT, June 2025
//

#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <GL/glew.h>
#include <string.h>

void renderText(const char* text, int x, int y, float scale, float color[3]);
void initTextRendering();

#endif