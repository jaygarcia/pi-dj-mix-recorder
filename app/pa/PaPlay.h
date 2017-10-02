//
// Created by jgarcia on 9/24/17.
//

#ifndef DJSR_CLION_PAPLAY_H
#define DJSR_CLION_PAPLAY_H

void stopAudio();
void pauseAudio();
void *  audioPlayback(void *ptr);
void preparePortAudio();
double getCurrentTimeStamp();

void openFile(char* fileNameToPlay);
void playTrack();
void cleanup();
short getPlaybackStatus();

#endif //DJSR_CLION_PAPLAY_H


