// Messages to display in the terminal and in the console
// By DREADCRAFT
//

/* See msg.h for more information */

#include "base.h"
#include "stdarg.h"
#include "msg.h"

/* Normal message */
void Msg(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), message, args);
    
    printf("%s", buffer);
    consoleAddOutput(buffer);
    
    va_end(args);
}

/* Normal message with colors */
void ColorMsg(float color[4], const char* message, ...)
{
    va_list args;
    va_start(args, message);
    
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), message, args);
    
    printf("%s", buffer);
    
    char colorBuffer[256];
    snprintf(colorBuffer, sizeof(colorBuffer), "^3%.2f,%.2f,%.2f,%.2f|%s", color[0], color[1], color[2], color[3], buffer);
    consoleAddOutput(colorBuffer);
    
    va_end(args);
}

/* Error message */
void Error(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), message, args);
    
    printf("%s", buffer);
    char errorBuffer[256];
    snprintf(errorBuffer, sizeof(errorBuffer), "^1%s", buffer);
    consoleAddOutput(errorBuffer);
    
    va_end(args);
}

/* Warning message */
void Warning(const char* message, ...)
{
    va_list args;
    va_start(args, message);
    
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), message, args);
    
    printf("%s", buffer);
    char warningBuffer[256];
    snprintf(warningBuffer, sizeof(warningBuffer), "^2%s", buffer);
    consoleAddOutput(warningBuffer);
    
    va_end(args);
}
