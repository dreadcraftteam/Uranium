// Drawing panel with image
// By DREADCRAFT
//

#include "base.h"
#include "stb/stb_image.h"
#include "imagepanel.h"

/* Drawing panel with image */
void drawImagePanel(ImagePanel* panel)
{
    if (!panel || !panel->isVisible || !panel->imagePath) return;

    GLboolean textureEnabled = glIsEnabled(GL_TEXTURE_2D);
    GLboolean lightingEnabled = glIsEnabled(GL_LIGHTING);
    float currentColor[4];
    glGetFloatv(GL_CURRENT_COLOR, currentColor);

    // Load texture if not already loaded
    if (panel->textureId == 0) 
    {
        Material* mat = loadMaterial(panel->imagePath);
        if (mat && mat->texture) 
        {
            panel->textureId = mat->texture->textureId;
        } 
        else 
        {
            return; // Failed to load texture
        }
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, panel->textureId);
    
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    
    glBegin(GL_QUADS);

        glTexCoord2f(0.0f, 0.0f); glVertex2f(panel->x, panel->y);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(panel->x + panel->width, panel->y);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(panel->x + panel->width, panel->y + panel->height);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(panel->x, panel->y + panel->height);

    glEnd();

    // Draw black outline if requested
    if (panel->outline) 
    {
        float black[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        glColor4fv(black);
        
        glDisable(GL_TEXTURE_2D);
        
        glBegin(GL_LINE_LOOP);

            glVertex2f(panel->x, panel->y);
            glVertex2f(panel->x + panel->width, panel->y);
            glVertex2f(panel->x + panel->width, panel->y + panel->height);
            glVertex2f(panel->x, panel->y + panel->height);

        glEnd();
        
        if (textureEnabled) 
        {
            glEnable(GL_TEXTURE_2D);
        }
    }
    
    if (!textureEnabled) 
    {
        glDisable(GL_TEXTURE_2D);
    }
    if (lightingEnabled) 
    {
        glEnable(GL_LIGHTING);
    } 
    else 
    {
        glDisable(GL_LIGHTING);
    }

    glColor4fv(currentColor);
}
