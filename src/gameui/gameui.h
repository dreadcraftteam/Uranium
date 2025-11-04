// GameUI is a graphical user interface system for the Uranium (engine)
// By DREADCRAFT
//

/* No, this is not a GameUI from the Source Engine :) */

#ifndef GAMEUI_H
#define GAMEUI_H

#include "base.h"

extern bool allowKeyboardInput();
extern bool allowMouseInput();

/* Set screen dimensions for orthographic projection */
void setScreenDimensions(int width, int height);

/* Begin GameUI rendering */
void beginGameUIRendering(void);

/* End GameUI rendering */
void endGameUIRendering(void);

/* GameUI initialize */
void gameuiInit(void);

/* GameUI updating */
void gameuiUpdate(void);

/* GameUI shutdown */
void gameuiShutdown(void);

#endif // GAMEUI_H
