// Keyboard and mouse input system
// By DREADCRAFT
//

#include "base.h"

#include "inputsystem.h"

/* Some variables */
static GLFWwindow* current_window = NULL;
static int* key_states = NULL;
static int* prev_key_states = NULL;
static int key_count = 0;

/* Input system initialization */
void inputSystemInit(GLFWwindow* frame) 
{
    current_window = frame;
    key_count = GLFW_KEY_LAST + 1;
    
    key_states = (int*)calloc(key_count, sizeof(int));
    prev_key_states = (int*)calloc(key_count, sizeof(int));
    
    for (int i = 0; i < key_count; i++) 
    {
        key_states[i] = GLFW_RELEASE;
        prev_key_states[i] = GLFW_RELEASE;
    }

    Msg("Input system initialized\n");
}

/* Updating */
void inputSystemUpdate() 
{
    for (int i = 0; i < key_count; i++) 
    {
        prev_key_states[i] = key_states[i];
        key_states[i] = glfwGetKey(current_window, i);
    }
}

/* Key down */
int KEY_DOWN(int key) 
{
    if (key < 0 || key >= key_count) return 0;
    return key_states[key] == GLFW_PRESS;
}

/* Check if key pressed */
int KEY_PRESSED(int key) 
{
    if (key < 0 || key >= key_count) return 0;
    return key_states[key] == GLFW_PRESS && prev_key_states[key] == GLFW_RELEASE;
}

/* Key released */
int KEY_RELEASED(int key) 
{
    if (key < 0 || key >= key_count) return 0;
    return key_states[key] == GLFW_RELEASE && prev_key_states[key] == GLFW_PRESS;
}

/* Mouse down*/
int MOUSE_DOWN(int button) 
{
    return glfwGetMouseButton(current_window, button) == GLFW_PRESS;
}

/* Getting mouse position */
void inputSystemGetMousePosition(double* xpos, double* ypos) 
{
    glfwGetCursorPos(current_window, xpos, ypos);
}

