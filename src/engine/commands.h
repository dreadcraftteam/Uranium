// Console commands for Uranium (on engine)
// By DREADCRAFT
//

#ifndef COMMANDS_H
#define COMMANDS_H

#include "base.h"

#include "console.h"
#include "variables.h"
#include "defines.h"

typedef struct 
{
    int key;
    char command[128];
} KeyBind;

extern KeyBind keyBinds[MAX_BINDS];
extern int bindCount;

/* Commands for console */
int consoleCommands(const char* commandLine);

#endif // COMMANDS_H
