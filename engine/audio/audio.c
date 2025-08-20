// Modified audio system by ThePixelMoon
// By DREADCRAFT and ThePixelMoon
// 

#include "base.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include "audio.h"

AudioSystem* audio = NULL;

struct AudioSystem
{
    ma_engine engine;
};

struct Sound
{
    ma_sound sound;
    char filename[256];
};

/* Creating system */
AudioSystem* audioSystemCreate(void)
{
    AudioSystem* sys = (AudioSystem*)malloc(sizeof(AudioSystem));
    if (!sys) return NULL;

    if (ma_engine_init(NULL, &sys->engine) != MA_SUCCESS) {
        free(sys);
        return NULL;
    }

    Msg("Sound initialized\n");

    return sys;
}

/* Destroying system */
void audioSystemDestroy(AudioSystem* sys)
{
    if (!sys) return;

    ma_engine_uninit(&sys->engine);

    Msg("Sound shutdown\n");

    free(sys);
}

/* Load the sound */
Sound* loadSound(AudioSystem* sys, const char* filename)
{
    if (!sys || !filename) return NULL;

    Sound* sound = (Sound*)malloc(sizeof(Sound));
    if (!sound) return NULL;

    char fullpath[256];
    snprintf(fullpath, sizeof(fullpath), "sound/%s", filename);

    if (ma_sound_init_from_file(&sys->engine, fullpath, 0, NULL, NULL, &sound->sound) != MA_SUCCESS)
    {
        Error("Failed to load sound: %s\n", fullpath);
    }

    strncpy(sound->filename, filename, sizeof(sound->filename)-1);
    sound->filename[sizeof(sound->filename)-1] = '\0';

    Msg("Loaded sound: %s\n", sound->filename);

    return sound;
}


/* Playing the sound */
void playSound(AudioSystem* sys, Sound* sound)
{
    if (!sys || !sound) return;
 
    ma_sound_start(&sound->sound);
}

/* Stopping the sound */
void stopSound(AudioSystem* sys, Sound* sound)
{
    if (!sys || !sound) return;
 
    Msg("Stopped sound: %s\n", sound);

    ma_sound_stop(&sound->sound);
}

/* Destroying the sound */
void audioSystemDestroySound(AudioSystem* sys, Sound* sound)
{
    if (!sys || !sound) return;
 
    ma_sound_uninit(&sound->sound);

    free(sound);
}
