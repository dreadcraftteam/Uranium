// GameUI is a graphical user interface system for the Uranium (engine)
// By DREADCRAFT
//

#include "base.h"

// GameUI controls 
#include "label.h"
#include "panel.h"
#include "imagepanel.h"

#include "gameui.h"

/* Screen dimensions */
static int screen_width = 800;
static int screen_height = 600;

/* Set screen dimensions for orthographic projection */
void setScreenDimensions(int width, int height)
{
    screen_width = width;
    screen_height = height;
}

/* Begin GameUI rendering */
void beginGameUIRendering()
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

/* End GameUI rendering */
void endGameUIRendering()
{
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

/* GameUI initialize */
void gameuiInit()
{
    labelInit();

    Msg("GameUI initialized\n");
}

/* GameUI updating */
void gameuiUpdate()
{
    
}

/* GameUI shutdown */
void gameuiShutdown()
{
    Msg("GameUI shutdown\n");
}
