// Video playback system for Uranium (engine)
// By DREADCRAFT
//

#include "base.h"

#include "video.h"

static int videoSystemInitialized = 0;

static void videoDecodeCallback(plm_t *plm, plm_frame_t *frame, void *user) 
{
    Video *video = (Video *)user;
    video->currentFrame = frame;

    if (video->currentFrame && video->textureId != 0) 
    {
        glBindTexture(GL_TEXTURE_2D, video->textureId);

        uint8_t *rgb_buffer = (uint8_t *)malloc(video->width * video->height * 3);
        if (rgb_buffer) 
        {
            plm_frame_to_rgb(video->currentFrame, rgb_buffer, video->width * 3);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, video->width, video->height, 0,
                        GL_RGB, GL_UNSIGNED_BYTE, rgb_buffer);
            free(rgb_buffer);
        }
    }
}

int videoInitSystem(void) 
{
    if (videoSystemInitialized) 
    {
        return 1;
    }

    videoSystemInitialized = 1;
    return 1;
}

void videoShutdownSystem(void) 
{
    videoSystemInitialized = 0;
}

Video* loadVideo(const char* filename, int auto_play) 
{
    if (!videoSystemInitialized) 
    {
        if (!Video_InitSystem()) 
        {
            return NULL;
        }
    }

    Video *video = (Video *)malloc(sizeof(Video));
    if (!video) 
    {
        return NULL;
    }

    memset(video, 0, sizeof(Video));
    strncpy(video->filename, filename, sizeof(video->filename) - 1);

    video->plm = plm_create_with_filename(filename);
    if (!video->plm) 
    {
        free(video);
    
        return NULL;
    }

    video->width = plm_get_width(video->plm);
    video->height = plm_get_height(video->plm);
    video->duration = plm_get_duration(video->plm);
    video->isLoaded = 1;

    plm_set_video_decode_callback(video->plm, videoDecodeCallback, video);

    plm_set_video_enabled(video->plm, 1);
    plm_set_audio_enabled(video->plm, 0);

    glGenTextures(1, &video->textureId);
    glBindTexture(GL_TEXTURE_2D, video->textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    uint8_t *empty_data = (uint8_t *)calloc(video->width * video->height * 3, 1);
    if (empty_data) 
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, video->width, video->height, 0,
                    GL_RGB, GL_UNSIGNED_BYTE, empty_data);
        free(empty_data);
    }

    if (auto_play) 
    {
        PlayVideo(video);
    }

    return video;
}

void unloadVideo(Video* video) 
{
    if (!video) return;

    if (video->textureId != 0) 
    {
        glDeleteTextures(1, &video->textureId);
    }

    if (video->plm) 
    {
        plm_destroy(video->plm);
    }

    free(video);
}

void playVideo(Video* video) 
{
    if (!video || !video->isLoaded) return;

    video->isPlaying = 1;
}

void pauseVideo(Video* video) 
{
    if (!video) return;

    video->isPlaying = 0;
}

void stopVideo(Video* video) 
{
    if (!video) return;

    video->isPlaying = 0;
    SeekVideo(video, 0.0);
}

void updateVideo(Video* video, double delta_time) 
{
    if (!video || !video->isLoaded || !video->isPlaying) return;

    plm_decode(video->plm, delta_time);
    video->currentTime = plm_get_time(video->plm);

    // Checking end of video...
    if (plm_has_ended(video->plm)) 
    {
        video->isPlaying = 0;
    }
}

void seekVideo(Video* video, double time) 
{
    if (!video || !video->isLoaded) return;

    plm_seek(video->plm, time, 0);
    video->currentTime = time;
}

int isVideoPlaying(Video* video) 
{
    return video && video->isPlaying;
}

int isVideoFinished(Video* video) 
{
    return video && video->isLoaded && plm_has_ended(video->plm);
}

double getVideoDuration(Video* video) 
{
    return video ? video->duration : 0.0;
}

double getVideoCurrentTime(Video* video) 
{
    return video ? video->currentTime : 0.0;
}

int getVideoWidth(Video* video) 
{
    return video ? video->width : 0;
}

int getVideoHeight(Video* video) 
{
    return video ? video->height : 0;
}
