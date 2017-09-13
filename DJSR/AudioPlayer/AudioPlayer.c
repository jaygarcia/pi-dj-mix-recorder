#include "AudioPlayer.h"

#define PCM_DEVICE "default"

//Based off of https://gist.github.com/ghedo/963382

struct SoundConfig {
    snd_pcm_t *pcm_handle;
    snd_pcm_hw_params_t *params;
    snd_pcm_uframes_t frames;

    int rate;
    int channels;
    int buff_size;
    char *buffer;
} SoundConfig;

// Configure ALSA audio
struct SoundConfig * setupAudio() {
    struct SoundConfig config;
 
    config.rate = 44100;
    config.channels = 2;
    unsigned int pcm, tmpVar, dir;

    int rate     = 44100,
        channels = 2,
        seconds  = 30;

    /* Open the PCM device in playback mode */
    if (pcm = snd_pcm_open(&config.pcm_handle, PCM_DEVICE, SND_PCM_STREAM_PLAYBACK, 0) < 0) {
        printf("ERROR: Can't open \"%s\" PCM device. %s\n", PCM_DEVICE, snd_strerror(pcm));
    }

    /* Allocate parameters object and fill it with default values*/
    snd_pcm_hw_params_alloca(&config.params);

    snd_pcm_hw_params_any(config.pcm_handle, config.params);

    /* Set parameters */
    if (pcm = snd_pcm_hw_params_set_access(config.pcm_handle, config.params, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) {
        printf("ERROR: Can't set interleaved mode. %s\n", snd_strerror(pcm));
    }

    if (pcm = snd_pcm_hw_params_set_format(config.pcm_handle, config.params, SND_PCM_FORMAT_S16_LE) < 0)  {
        printf("ERROR: Can't set format. %s\n", snd_strerror(pcm));
    }

    if (pcm = snd_pcm_hw_params_set_channels(config.pcm_handle, config.params, channels) < 0)  {
        printf("ERROR: Can't set channels number. %s\n", snd_strerror(pcm));
    }

    if (pcm = snd_pcm_hw_params_set_rate_near(config.pcm_handle, config.params, &rate, 0) < 0) { 
        printf("ERROR: Can't set rate. %s\n", snd_strerror(pcm));
    }
    
    /* Write parameters */
    if (pcm = snd_pcm_hw_params(config.pcm_handle, config.params) < 0) {
        printf("ERROR: Can't set hardware parameters. %s\n", snd_strerror(pcm));
    }

    /* Resume information */
    printf("PCM name: '%s'\n", snd_pcm_name(config.pcm_handle));

    printf("PCM state: %s\n", snd_pcm_state_name(snd_pcm_state(config.pcm_handle)));

    snd_pcm_hw_params_get_channels(config.params, &tmpVar);
    printf("channels: %u ", tmpVar);

    if (tmpVar == 1) {
        printf("(mono)\n");
    }
    else if (tmpVar == 2) {
        printf("(stereo)\n");
    }
    
    snd_pcm_hw_params_get_rate(config.params, &tmpVar, 0);
    printf("rate: %u bps\n", tmpVar);

    printf("seconds: %d\n", seconds);	

    /* Allocate buffer to hold single period */
    snd_pcm_hw_params_get_period_size(config.params, &config.frames, 0);

    
    config.buff_size = config.frames * channels * 2 /* 2 -> sample size */;
    config.buffer = (char *) malloc(config.buff_size);

    snd_pcm_hw_params_get_period_time(config.params, &tmpVar, NULL);
    
    return &config;
}

int playAudioTest(char *file) {
    printf("Playing audio...\n");;
        
    printf("File: %s\n", file);
    
    
    
    struct SoundConfig * soundCfg = setupAudio();

    
    return 0;
//    for (loops = (seconds * 1000000) / tmpVar; loops > 0; loops--) {
//
//        if (pcm = read(0, buff, buff_size) == 0) {
//            printf("Early end of file.\n");
//            return 0;
//        }
//
//        if (pcm = snd_pcm_writei(pcm_handle, buff, frames) == -EPIPE) {
//            printf("XRUN.\n");
//            snd_pcm_prepare(pcm_handle);
//        }
//        else if (pcm < 0) {
//            printf("ERROR. Can't write to PCM device. %s\n", snd_strerror(pcm));
//        }
//
//    }
//
//    snd_pcm_drain(pcm_handle);
//    snd_pcm_close(pcm_handle);
//    free(buff);
    
    return 0;
}