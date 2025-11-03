// Messages to display in the terminal and in the console
// By DREADCRAFT
//

/* Yes, I took the idea from the Source Engine, so what? */

#ifndef MSG_H
#define MSG_H

/* Normal message */
void Msg(const char* message, ...);

/* Error message */
void Error(const char* message, ...);

/* Warning message */
void Warning(const char* message, ...);

#endif // MSG_H