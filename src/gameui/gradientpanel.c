// Drawing panel with gradients 
// By DREADCRAFT
//

#include "base.h"
#include "gradientpanel.h"

/* Drawing panel with Gradient */
void drawGradientPanel(GradientPanel* panel)
{
    if (!panel || !panel->isVisible) return;

    glPushAttrib(GL_CURRENT_BIT);
    
    glBegin(GL_QUADS);

        glColor4fv(panel->firstColor);
        glVertex2f(panel->x, panel->y);
        
        glColor4fv(panel->firstColor);
        glVertex2f(panel->x + panel->width, panel->y);
        
        glColor4fv(panel->secondColor);
        glVertex2f(panel->x + panel->width, panel->y + panel->height);
        
        glColor4fv(panel->secondColor);
        glVertex2f(panel->x, panel->y + panel->height);

    glEnd();

    // Draw black outline if requested
    if (panel->outline) 
    {
        float black[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        glColor4fv(black);
        
        glBegin(GL_LINE_LOOP);

            glVertex2f(panel->x, panel->y);
            glVertex2f(panel->x + panel->width, panel->y);
            glVertex2f(panel->x + panel->width, panel->y + panel->height);
            glVertex2f(panel->x, panel->y + panel->height);

        glEnd();
    }
    
    glPopAttrib();
}
