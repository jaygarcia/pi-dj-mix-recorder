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
#include <sys/stat.h>

SNDFILE *infile;
SF_INFO sfinfo ;
int currentByteRate;
PaStream *stream;
PaStreamParameters outputParameters;
pthread_mutex_t pausePlayStopMutex,
                timestampMutex;

double currentTimeStamp;

off_t fileSize;

unsigned long totalBytesRead,
              totalFramesRead;

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


unsigned long fsize(char* file) {
    FILE * f = fopen(file, "r");
    fseek(f, 0, SEEK_END);
    unsigned long len = (unsigned long)ftell(f);
    fclose(f);
    return len;
}


void openFile(char* fileNameToOpen) {

    /** LOCK **/
    pthread_mutex_lock(&pausePlayStopMutex);

    PlayerState.fileLoading = true;
    free(PlayerState.fileName);
    realloc(PlayerState.fileName, sizeof(char) * strlen(fileNameToOpen));
    PlayerState.fileName = strdup(fileNameToOpen);


    struct stat fileStat;

    if (stat(fileNameToOpen, &fileStat) != 0 || (!S_ISREG(fileStat.st_mode))) {
        fprintf(stderr, "ERROR :: Could no open file %s\n", fileNameToOpen);
        pthread_mutex_unlock(&pausePlayStopMutex);

        return;
    }

    fileSize = fileStat.st_size;

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

    currentByteRate = sf_current_byterate(infile);

    puts(("Opened file %s.", fileNameToOpen));
    printf("\tByte Rate %i \n", currentByteRate);
    printf("\tNum frames: %lu\n", sfinfo.frames);
    printf("\tChannels : %i\n", sfinfo.channels);
    printf("\tSample Rate: %i\n", sfinfo.samplerate);

    // Do we want to support more than one format?
    printf("\tFormat: 0x%x ", SF_FORMAT_TYPEMASK & sfinfo.format);
    fflush(stdout);

    if ((SF_FORMAT_TYPEMASK & sfinfo.format) == SF_FORMAT_WAV) {
        printf("Microsoft WAV format (little endian). ");
    }

    if ((SF_FORMAT_SUBMASK & sfinfo.format) == SF_FORMAT_PCM_16) {
        printf("Signed 16 bit");
    }
    printf("\n");

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


    // TODO: Hard coding 4 is BAD! Need to figure out how to programmatically get bytes per frame
    memset(out, 0x00, framesPerBuffer * sfinfo.channels * 4);

    // Allow the buffer to stay silent when paused
    if (state == PAUSED) {
        printf("state == Paused\n");
        return paContinue;
    }

    /* Read with libsndfile */
    readcount = sf_read_float(infile, out, framesPerBuffer * 2);

    pthread_mutex_lock(&timestampMutex);

    totalFramesRead += framesPerBuffer;

    pthread_mutex_unlock(&timestampMutex);


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
        printf("Frames read %lu out of %lu total\n", totalFramesRead, sfinfo.frames);
        fflush(stdout);

        currentTimeStamp = ts;
        return ts;
    }
    else {
        return currentTimeStamp;
    }

}

int findUsbAudio() {
    int numDevices = Pa_GetDeviceCount(),
        i = 0;

    const PaDeviceInfo *deviceInfo;
    char *queryStr = "USB Audio CODEC";

    for (; i < numDevices; i++) {
        deviceInfo = Pa_GetDeviceInfo(i);

        if (strstr(deviceInfo->name,  "USB Audio CODEC") != NULL) {
            return i;
        }
    }

    return -1;
}

void preparePortAudio() {
    stream = NULL;
    PaError retval;
    struct sigaction sa;

    totalBytesRead = 0;
    totalFramesRead = 0;

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
//        const PaDeviceInfo *playbackDeviceInfo;
//        playbackDeviceInfo = Pa_GetDeviceInfo(outputParameters.device);

        if (outputParameters.device == paNoDevice) {
            fprintf(stderr, "Error: No default output device.\n");
            goto exit;
        }

        int usbAudioDeviceIndex = findUsbAudio();

        if (usbAudioDeviceIndex == -1) {
            fprintf(stderr, "ERROR :: Could not find USB audio device. Aborting.\n");
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



