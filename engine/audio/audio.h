// Modified audio system by ThePixelMoon
// By DREADCRAFT and ThePixelMoon
// 

#ifndef AUDIO_H
#define AUDIO_H

#include "base.h"

typedef struct AudioSystem AudioSystem;
typedef struct Sound Sound;

/* Creating system */
AudioSystem* audioSystemCreate(void);

/* Destroying system */
void audioSystemDestroy(AudioSystem* sys);

/* Load the sound */
Sound* loadSound(AudioSystem* sys, const char* filename);

/* Playing the sound */
void playSound(AudioSystem* sys, Sound* sound);

/* Stopping the sound */
void stopSound(AudioSystem* sys, Sound* sound);

/* Destroying the sound */
void audioSystemDestroySound(AudioSystem* sys, Sound* sound);

extern AudioSystem* audio;

#endif // AUDIO_H

