// Header for keyboard and mouse input system
// By DREADCRAFT
//

#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

/*

    KEY_DOWN - the key is currently pressed.

    KEY_PRESSED - the key has just been pressed (once when pressed)

    KEY_RELEASED - the key has just been released.

*/

#include "base.h"

#include <GLFW/glfw3.h>

/* Printable keys */
#define INPUT_KEY_SPACE          GLFW_KEY_SPACE
#define INPUT_KEY_APOSTROPHE     GLFW_KEY_APOSTROPHE 
#define INPUT_KEY_COMMA          GLFW_KEY_COMMA     
#define INPUT_KEY_MINUS          GLFW_KEY_MINUS 
#define INPUT_KEY_PERIOD         GLFW_KEY_PERIOD      
#define INPUT_KEY_SLASH          GLFW_KEY_SLASH
#define INPUT_KEY_0              GLFW_KEY_0
#define INPUT_KEY_1              GLFW_KEY_1
#define INPUT_KEY_2              GLFW_KEY_2
#define INPUT_KEY_3              GLFW_KEY_3
#define INPUT_KEY_4              GLFW_KEY_4
#define INPUT_KEY_5              GLFW_KEY_5
#define INPUT_KEY_6              GLFW_KEY_6
#define INPUT_KEY_7              GLFW_KEY_7
#define INPUT_KEY_8              GLFW_KEY_8
#define INPUT_KEY_9              GLFW_KEY_9
#define INPUT_KEY_SEMICOLON      GLFW_KEY_SEMICOLON    
#define INPUT_KEY_EQUAL          GLFW_KEY_EQUAL        
#define INPUT_KEY_A              GLFW_KEY_A
#define INPUT_KEY_B              GLFW_KEY_B
#define INPUT_KEY_C              GLFW_KEY_C
#define INPUT_KEY_D              GLFW_KEY_D
#define INPUT_KEY_E              GLFW_KEY_E
#define INPUT_KEY_F              GLFW_KEY_F
#define INPUT_KEY_G              GLFW_KEY_G
#define INPUT_KEY_H              GLFW_KEY_H
#define INPUT_KEY_I              GLFW_KEY_I
#define INPUT_KEY_J              GLFW_KEY_J
#define INPUT_KEY_K              GLFW_KEY_K
#define INPUT_KEY_L              GLFW_KEY_L
#define INPUT_KEY_M              GLFW_KEY_M
#define INPUT_KEY_N              GLFW_KEY_N
#define INPUT_KEY_O              GLFW_KEY_O
#define INPUT_KEY_P              GLFW_KEY_P
#define INPUT_KEY_Q              GLFW_KEY_Q
#define INPUT_KEY_R              GLFW_KEY_R
#define INPUT_KEY_S              GLFW_KEY_S
#define INPUT_KEY_T              GLFW_KEY_T
#define INPUT_KEY_U              GLFW_KEY_U
#define INPUT_KEY_V              GLFW_KEY_V
#define INPUT_KEY_W              GLFW_KEY_W
#define INPUT_KEY_X              GLFW_KEY_X
#define INPUT_KEY_Y              GLFW_KEY_Y
#define INPUT_KEY_Z              GLFW_KEY_Z
#define INPUT_KEY_LEFT_BRACKET   GLFW_KEY_LEFT_BRACKET 
#define INPUT_KEY_BACKSLASH      GLFW_KEY_BACKSLASH    
#define INPUT_KEY_RIGHT_BRACKET  GLFW_KEY_RIGHT_BRACKET
#define INPUT_KEY_GRAVE_ACCENT   GLFW_KEY_GRAVE_ACCENT 
#define INPUT_KEY_WORLD_1        GLFW_KEY_WORLD_1    
#define INPUT_KEY_WORLD_2        GLFW_KEY_WORLD_2      

/* Function keys */
#define INPUT_KEY_ESCAPE        GLFW_KEY_ESCAPE
#define INPUT_KEY_ENTER         GLFW_KEY_ENTER
#define INPUT_KEY_TAB           GLFW_KEY_TAB
#define INPUT_KEY_BACKSPACE     GLFW_KEY_BACKSPACE
#define INPUT_KEY_INSERT        GLFW_KEY_INSERT
#define INPUT_KEY_DELETE        GLFW_KEY_DELETE
#define INPUT_KEY_RIGHT         GLFW_KEY_RIGHT
#define INPUT_KEY_LEFT          GLFW_KEY_LEFT
#define INPUT_KEY_DOWN          GLFW_KEY_DOWN
#define INPUT_KEY_UP            GLFW_KEY_UP
#define INPUT_KEY_PAGE_UP       GLFW_KEY_PAGE_UP
#define INPUT_KEY_PAGE_DOWN     GLFW_KEY_PAGE_DOWN
#define INPUT_KEY_HOME          GLFW_KEY_HOME
#define INPUT_KEY_END           GLFW_KEY_END
#define INPUT_KEY_CAPS_LOCK     GLFW_KEY_CAPS_LOCK
#define INPUT_KEY_SCROLL_LOCK   GLFW_KEY_SCROLL_LOCK
#define INPUT_KEY_NUM_LOCK      GLFW_KEY_NUM_LOCK
#define INPUT_KEY_PRINT_SCREEN  GLFW_KEY_PRINT_SCREEN
#define INPUT_KEY_PAUSE         GLFW_KEY_PAUSE
#define INPUT_KEY_F1            GLFW_KEY_F1
#define INPUT_KEY_F2            GLFW_KEY_F2
#define INPUT_KEY_F3            GLFW_KEY_F3
#define INPUT_KEY_F4            GLFW_KEY_F4
#define INPUT_KEY_F5            GLFW_KEY_F5
#define INPUT_KEY_F6            GLFW_KEY_F6
#define INPUT_KEY_F7            GLFW_KEY_F7
#define INPUT_KEY_F8            GLFW_KEY_F8
#define INPUT_KEY_F9            GLFW_KEY_F9
#define INPUT_KEY_F10           GLFW_KEY_F10
#define INPUT_KEY_F11           GLFW_KEY_F11
#define INPUT_KEY_F12           GLFW_KEY_F12
#define INPUT_KEY_F13           GLFW_KEY_F13
#define INPUT_KEY_F14           GLFW_KEY_F14
#define INPUT_KEY_F15           GLFW_KEY_F15
#define INPUT_KEY_F16           GLFW_KEY_F16
#define INPUT_KEY_F17           GLFW_KEY_F17
#define INPUT_KEY_F18           GLFW_KEY_F18
#define INPUT_KEY_F19           GLFW_KEY_F19
#define INPUT_KEY_F20           GLFW_KEY_F20
#define INPUT_KEY_F21           GLFW_KEY_F21
#define INPUT_KEY_F22           GLFW_KEY_F22
#define INPUT_KEY_F23           GLFW_KEY_F23
#define INPUT_KEY_F24           GLFW_KEY_F24
#define INPUT_KEY_F25           GLFW_KEY_F25
#define INPUT_KEY_KP_0          GLFW_KEY_KP_0
#define INPUT_KEY_KP_1          GLFW_KEY_KP_1
#define INPUT_KEY_KP_2          GLFW_KEY_KP_2
#define INPUT_KEY_KP_3          GLFW_KEY_KP_3
#define INPUT_KEY_KP_4          GLFW_KEY_KP_4
#define INPUT_KEY_KP_5          GLFW_KEY_KP_5
#define INPUT_KEY_KP_6          GLFW_KEY_KP_6
#define INPUT_KEY_KP_7          GLFW_KEY_KP_7
#define INPUT_KEY_KP_8          GLFW_KEY_KP_8
#define INPUT_KEY_KP_9          GLFW_KEY_KP_9
#define INPUT_KEY_KP_DECIMAL    GLFW_KEY_KP_DECIMAL
#define INPUT_KEY_KP_DIVIDE     GLFW_KEY_KP_DIVIDE
#define INPUT_KEY_KP_MULTIPLY   GLFW_KEY_KP_MULTIPLY
#define INPUT_KEY_KP_SUBTRACT   GLFW_KEY_KP_SUBTRACT
#define INPUT_KEY_KP_ADD        GLFW_KEY_KP_ADD
#define INPUT_KEY_KP_ENTER      GLFW_KEY_KP_ENTER
#define INPUT_KEY_KP_EQUAL      GLFW_KEY_KP_EQUAL
#define INPUT_KEY_LEFT_SHIFT    GLFW_KEY_LEFT_SHIFT
#define INPUT_KEY_LEFT_CONTROL  GLFW_KEY_LEFT_CONTROL
#define INPUT_KEY_LEFT_ALT      GLFW_KEY_LEFT_ALT
#define INPUT_KEY_LEFT_SUPER    GLFW_KEY_LEFT_SUPER
#define INPUT_KEY_RIGHT_SHIFT   GLFW_KEY_RIGHT_SHIFT
#define INPUT_KEY_RIGHT_CONTROL GLFW_KEY_RIGHT_CONTROL
#define INPUT_KEY_RIGHT_ALT     GLFW_KEY_RIGHT_ALT
#define INPUT_KEY_RIGHT_SUPER   GLFW_KEY_RIGHT_SUPER
#define INPUT_KEY_MENU          GLFW_KEY_MENU

/* Mouse button definitions */
#define INPUT_MOUSE_LEFT        GLFW_MOUSE_BUTTON_LEFT
#define INPUT_MOUSE_RIGHT       GLFW_MOUSE_BUTTON_RIGHT
#define INPUT_MOUSE_MIDDLE      GLFW_MOUSE_BUTTON_MIDDLE
#define INPUT_MOUSE_4           GLFW_MOUSE_BUTTON_4
#define INPUT_MOUSE_5           GLFW_MOUSE_BUTTON_5
#define INPUT_MOUSE_6           GLFW_MOUSE_BUTTON_6
#define INPUT_MOUSE_7           GLFW_MOUSE_BUTTON_7
#define INPUT_MOUSE_8           GLFW_MOUSE_BUTTON_8

/* Modifier key flags (for combination checking) */
#define MOD_SHIFT               GLFW_MOD_SHIFT
#define MOD_CONTROL             GLFW_MOD_CONTROL
#define MOD_ALT                 GLFW_MOD_ALT
#define MOD_SUPER               GLFW_MOD_SUPER
#define MOD_CAPS_LOCK           GLFW_MOD_CAPS_LOCK
#define MOD_NUM_LOCK            GLFW_MOD_NUM_LOCK

/* Input system initialization */
void inputSystemInit(GLFWwindow* frame);

/* Updating */
void inputSystemUpdate(void);

/* Check if key pressed */
int KEY_PRESSED(int key);

/* Key released */
int KEY_RELEASED(int key);

/* Key down */
int KEY_DOWN(int key);

/* Mouse down*/
int MOUSE_DOWN(int button);

/* Getting mouse position */
void inputSystemGetMousePosition(double* xpos, double* ypos);

/* Shutdown */
void inputSystemShutdown(void);

#endif // INPUTSYSTEM_H
