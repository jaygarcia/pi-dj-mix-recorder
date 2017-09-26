//
// Created by jgarcia on 9/24/17.
//

#include "PaPlay.h"

#define _XOPEN_SOURCE
#define _POSIX_C_SOURCE 199309L

#define PA_ENABLE_DEBUG_OUTPUT 1

#include <string.h>
#include <stdio.h>
#include <sndfile.h>
#include <signal.h>
#include <pthread.h>
#include <malloc.h>
#include <stdlib.h>
#include <portaudio.h>
#include <pa_linux_alsa.h>
#include <unistd.h>
#include <sys/time.h>

SNDFILE *infile;
SF_INFO sfinfo ;
PaStream *stream;
PaStreamParameters outputParameters;
pthread_mutex_t operationMutex;
/* To be locked via mutex */
char* fileName;
short isPlaying = 0; // 1 = yes, 0 = no;



void stopAudio() {
    pthread_mutex_lock(&operationMutex);
    isPlaying = 0;
    pthread_mutex_unlock(&operationMutex);
}


void openFile(char* fileNameToOpen) {

    /** LOCK **/
    pthread_mutex_lock(&operationMutex);
    free(fileName);
    realloc(fileName, sizeof(char) * strlen(fileNameToOpen));
    fileName = strdup(fileNameToOpen);
    /** UNLOCK **/
    pthread_mutex_unlock(&operationMutex);

    /* Open file. Because this is just a example we asume
        What you are doing and give file first argument */
    if (! (infile = sf_open(fileName, SFM_READ, &sfinfo))) {
        printf("Not able to open input file %s.\n", fileNameToOpen) ;
        sf_perror(NULL) ;
        return ;
    }
    else {
        printf("Opened file %s.\n", fileNameToOpen);
    }

    preparePortAudio();
}

void playTrack() {
    if (isPlaying == 1)  {
        printf("Sorry Jack! Already playing %s\n", fileName);
        return;
    }

    if (fileName == NULL) {
        puts("No file is open!!");
        return;
    }

    pthread_mutex_lock(&operationMutex);

    isPlaying = 1;

    pthread_mutex_unlock(&operationMutex);
    audioPlayback(NULL);


//    pthread_t playThread;
//    pthread_create(&playThread, NULL, audioPlayback, NULL);
}


/* Reques for writing length data */
static int streamCallback(const void *inputBuffer,
                          void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData) {

    pthread_mutex_lock(&operationMutex);
    short playing = isPlaying;
    pthread_mutex_unlock(&operationMutex);

    if (playing == 0) {
        return paComplete;
    }

    float *out = (float*)outputBuffer;
    long readcount = 0;

    memset(out, 0x00, framesPerBuffer);

    /* Read with libsndfile */
    readcount = sf_read_float(infile, out, framesPerBuffer * 2);

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
        goto exit;
    }
    else {
        puts("Stream open...");
        fflush(stdout);
    }


    retval = Pa_StartStream(stream);
    if (retval != paNoError) {
        cleanup();
    }
    puts("end of audioPlayback");

    if (retval == paNoError) {
        return;
    }


exit:
    /* clean up and disconnect */
    cleanup();
    return NULL;
}

void preparePortAudio() {
    stream = NULL;
    PaError retval = 0;
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


    puts("Pa_Initialize()");
    fflush(stdout);
    retval = Pa_Initialize();
    puts("END Pa_Initialize()");
    fflush(stdout);

    if (retval != paNoError) {
        printf("Could not initialize PortAudio!\n");
        goto exit;
    }

    puts("Pa_GetDefaultOutputDevice()");
    fflush(stdout);
    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */

    puts("END Pa_GetDefaultOutputDevice();");
    fflush(stdout);
    if (outputParameters.device == paNoDevice) {
        fprintf(stderr, "Error: No default output device.\n");
        goto exit;
    }

    outputParameters.channelCount = 2;       /* stereo output */
    outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    if (retval == paNoError) {
        return;
    }

exit:
    /* clean up and disconnect */
    cleanup();
}

void cleanup() {
    PaError retval;

    retval = Pa_StopStream(stream);
    if (retval != paNoError) {
        goto exit;
    }

    retval = Pa_CloseStream(stream);

    if (retval != paNoError) {
        goto exit;
    }

    Pa_Terminate();


    exit:
    /* clean up and disconnect */
    printf("\nExit and clean\n");
    sf_close(infile);
    Pa_Terminate();

    isPlaying = 0;
    printf("Cleanup done...\n");

}



