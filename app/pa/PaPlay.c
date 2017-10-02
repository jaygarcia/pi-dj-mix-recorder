//
// Created by jgarcia on 9/24/17.
//

#include "PaPlay.h"

#define _XOPEN_SOURCE
#define _POSIX_C_SOURCE 199309L

#define PA_ENABLE_DEBUG_OUTPUT 1

#include <memory.h>
#include <stdio.h>
#include <sndfile.h>
#include <signal.h>
#include <pthread.h>
#include <malloc.h>
#include <stdlib.h>
#include <portaudio.h>
#include <stdbool.h>
#include <pa_linux_alsa.h>
#include <unistd.h>
#include <sys/time.h>

SNDFILE *infile;
SF_INFO sfinfo ;
PaStream *stream;
PaStreamParameters outputParameters;
pthread_mutex_t pausePlayStopMutex,
                timestampMutex;

double currentTimeStamp;

int fileByteRate;
/* To be locked via mutex */


#define STOPPED 0
#define PLAYING 1
#define PAUSED  2

struct PlayerState {
    char *fileName;
    bool fileLoaded;
    bool portAudioInitialized;
    bool fileLoading;
    /**
     * 0    Stopped
     * 1    Playing
     * 2    Paused
     */
    short state;
} PlayerState;

void setPlayerStateUsingMutex(short newState) {
    pthread_mutex_lock(&pausePlayStopMutex);

    PlayerState.state = newState;

    pthread_mutex_unlock(&pausePlayStopMutex);
}

short getPlayerStateUsingMutex() {
    pthread_mutex_lock(&pausePlayStopMutex);
    short state = PlayerState.state;
    pthread_mutex_unlock(&pausePlayStopMutex);
    return state;
}

void openFile(char* fileNameToOpen) {

    /** LOCK **/
    pthread_mutex_lock(&pausePlayStopMutex);

    PlayerState.fileLoading = true;
    free(PlayerState.fileName);
    realloc(PlayerState.fileName, sizeof(char) * strlen(fileNameToOpen));
    PlayerState.fileName = strdup(fileNameToOpen);

    /** UNLOCK **/
    pthread_mutex_unlock(&pausePlayStopMutex);

    /* Open file. Because this is just a example we asume
        What you are doing and give file first argument */
    if (! (infile = sf_open(fileNameToOpen, SFM_READ, &sfinfo))) {
        puts(("Not able to open input file %s.\n", fileNameToOpen));
        sf_perror(NULL) ;
        return ;
    }





    preparePortAudio();

    pthread_mutex_lock(&pausePlayStopMutex);
    PlayerState.fileLoaded = true;
    PlayerState.fileLoading = false;
    pthread_mutex_unlock(&pausePlayStopMutex);

    puts(("Opened file %s.\n", fileNameToOpen));
    printf("\n\nfileByteRate 0x%08X \n", sfinfo.format | SF_FORMAT_WAV);

}

void playTrack() {
    short state = getPlayerStateUsingMutex();

    if (PlayerState.fileLoading == true) {
        puts("File still loading. Aborting!");
        return;
    }

    if (state == PLAYING)  {
        puts(("Sorry Jack! Already playing %s", PlayerState.fileName));
        return;
    }

    if (state == PAUSED) {
        puts("Resuming");
        setPlayerStateUsingMutex(PLAYING);
        audioPlayback(NULL);
        return;
    }

    if (PlayerState.fileLoaded == false) {
        puts("No file is loaded!");
        return;
    }

    if (PlayerState.fileName == NULL) {
        puts("No file is open!!");
        return;
    }

    setPlayerStateUsingMutex(PLAYING);
    audioPlayback(NULL);
}


/* Reques for writing length data */
static int streamCallback(const void *inputBuffer,
                          void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData) {

    short state = getPlayerStateUsingMutex();


    float *out = (float*)outputBuffer;
    long readcount = 0;

    printf("outputBufferDacTime == %lf\n", Pa_GetStreamTime(stream) - timeInfo->outputBufferDacTime);
    fflush(stdout);

    // TODO: Hard coding 4 is BAD! Need to figure out how to programmatically get bytes per frame
    memset(out, 0x00, framesPerBuffer * sfinfo.channels * 4);

    // Allow the buffer to stay silent when paused
    if (state == PAUSED) {
        printf("state == Paused\n");
        return paContinue;
    }

    /* Read with libsndfile */
    readcount = sf_read_float(infile, out, framesPerBuffer * 2);

    //printf("read %lu bytes from %s\n", readcount, PlayerState.fileName);

    /* File end if we read -1 */
    if (readcount <= 0) {
        printf("File has ended!\n");
        return paComplete;
    }

    return paContinue;
}

/* Handle termination with CTRL-C */
static void handler(int sig, siginfo_t *si, void *unused) {
    printf("Got SIGSEGV at address: 0x%lx\n", (long) si->si_addr);
}

void * audioPlayback(void *ptr) {
    PaError retval;

    puts("Attempting to open stream...");
    fflush(stdout);

    retval = Pa_OpenStream(
            &stream,                //
            NULL, /* no input */
            &outputParameters,
            44100,
            1024 * 8,
            paClipOff,      /* we won't output out of range samples so don't bother clipping them */
            streamCallback,
            infile);

    if (retval != paNoError) {
        puts("Could not open stream, executing cleanup()");
        goto exit;
    }

    retval = Pa_StartStream(stream);
    if (retval != paNoError) {
        goto exit;
    }

    if (retval == paNoError) {
        return NULL;
    }


exit:
    /* clean up and disconnect */
    cleanup();
    return NULL;
}

short getPlaybackStatus() {
    pthread_mutex_lock(&pausePlayStopMutex);
    short state = PlayerState.state;
    pthread_mutex_unlock(&pausePlayStopMutex);

    return state;
}

double getCurrentTimeStamp() {
    if (Pa_IsStreamActive(stream) == 1) {
        double ts = (double) Pa_GetStreamTime(stream);
        printf("ts == %lf\n", ts);
        currentTimeStamp = ts;
        return ts;
    }
    else {
        return currentTimeStamp;
    }

}

void preparePortAudio() {
    stream = NULL;
    PaError retval;
    struct sigaction sa;

    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = handler;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        printf("Can't set SIGINT handler!\n");
        sf_close(infile);
        return;
    }

    if (sigaction(SIGHUP, &sa, NULL) == -1) {
        printf("Can't set SIGHUP handler!\n");
        sf_close(infile);
        return;
    }


    if (! PlayerState.portAudioInitialized) {
        retval = Pa_Initialize();

        if (retval != paNoError) {
            printf("Could not initialize PortAudio!\n");
            goto exit;
        }

        pthread_mutex_lock(&pausePlayStopMutex);
        PlayerState.portAudioInitialized = true;
        pthread_mutex_unlock(&pausePlayStopMutex);

        outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
        const PaDeviceInfo *playbackDeviceInfo;
        playbackDeviceInfo = Pa_GetDeviceInfo( outputParameters.device );

        if (outputParameters.device == paNoDevice) {
            fprintf(stderr, "Error: No default output device.\n");
            goto exit;
        }

        outputParameters.channelCount = 2;       /* stereo output */
        outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
        outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
        outputParameters.hostApiSpecificStreamInfo = NULL;

        puts("Port Audio initialized. Ready for playback!");
        return;

    }
    else {
        return;
    }

exit:
    /* clean up and disconnect */
    cleanup();
}



void pauseAudio() {
    setPlayerStateUsingMutex(PAUSED);
    Pa_StopStream(stream);
}

void stopAudio() {
    setPlayerStateUsingMutex(STOPPED);
    Pa_StopStream(stream);
    sf_seek(infile, 0,0);
}


void cleanup() {
    puts("Cleanup started...");

    PaError retval;

    retval = Pa_StopStream(stream);
    if (retval != paNoError) {
        goto exit;
    }

    retval = Pa_CloseStream(stream);

    if (retval != paNoError) {
        goto exit;
    }


exit:
    /* clean up and disconnect */
    puts("Terminating PortAudio session");
    Pa_Terminate();
    Pa_Sleep(40);

    sf_close(infile);

    setPlayerStateUsingMutex(STOPPED);
    puts("Cleanup done...");

}



