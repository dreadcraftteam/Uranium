// Video playback system for Uranium (engine)
// By DREADCRAFT
//

#ifndef VIDEO_H
#define VIDEO_H

#include "base.h"

#include "pl_mpeg/pl_mpeg.h"

typedef struct 
{
    char filename[256];
    plm_t *plm;
    plm_frame_t *currentFrame;
    GLuint textureId;
    int width;
    int height;
    int isPlaying;
    int isLoaded;
    double duration;
    double currentTime;
} Video;

/* Control system of video */
int videoInitSystem(void);
void videoShutdownSystem(void);

/* Loading video */
Video* loadVideo(const char* filename, int auto_play);

/* Unloading video */
void unloadVideo(Video* video);

/* Playing video */
void playVideo(Video* video);

/* Pause */
void pauseVideo(Video* video);

/* Stopping video */
void stopVideo(Video* video);

/* Updating video */
void ppdateVideo(Video* video, double delta_time);

/* Seeking video */
void seekVideo(Video* video, double time);

int isVideoPlaying(Video* video);
int isVideoFinished(Video* video);

double getVideoDuration(Video* video);
double getVideoCurrentTime(Video* video);
int getVideoWidth(Video* video);
int getVideoHeight(Video* video);

#endif // VIDEO_H
