// Drawing normal panel (with colors)
// By DREADCRAFT
//

#include "base.h"
#include "panel.h"

/* Drawing basic gray panel */
void drawPanel(Panel* panel)
{
    if (!panel || !panel->isVisible) return;

    glPushAttrib(GL_CURRENT_BIT);

    // Draw main panel
    glColor4fv(panel->color);
    
    glBegin(GL_QUADS);

        glVertex2f(panel->x, panel->y);
        glVertex2f(panel->x + panel->width, panel->y);
        glVertex2f(panel->x + panel->width, panel->y + panel->height);
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
