/* 
 * File:   main.c
 * Author: jgarcia
 *
 * Created on September 13, 2017, 9:34 AM
 */

#include <stdio.h>
#include <stdlib.h>                     
#include "AudioPlayer/AudioPlayer.h"

#include <pthread.h>
#include <unistd.h>

#include "app/app.h"

void setupSignalHandlers();
void nonfatal_sighandler();
void exit_sighandler();

int main(int argc, char** argv) {

    appStart();
    
    //This is just for testing.
    //audioAction("/home/jgarcia/projects/djsr/song.wav", "play");
    
    return 0;
}
//void setupSignalHandlers() {
//    // First setup signal handler
//    signal(SIGPIPE, nonfatal_sighandler); // PIPE (socket control)
//    signal(SIGTERM, exit_sighandler); // kill
//    signal(SIGHUP,  exit_sighandler); // kill -HUP / xterm closed
//    signal(SIGINT,  exit_sighandler); // Interrupt from keyboard
//    signal(SIGQUIT, exit_sighandler); // Quit from keyboard
//    // fatal errors
//    signal(SIGBUS,  exit_sighandler); // bus error
//    //signal(SIGSEGV, exit_sighandler);	// segfault
//    signal(SIGILL,  exit_sighandler); // illegal instruction
//    signal(SIGFPE,  exit_sighandler); // floating point exc.
//    signal(SIGABRT, exit_sighandler); // abort()
//}
//
//
//void nonfatal_sighandler(int x) {
//    printf("Warning: alsaplayer interrupted by signal %d\n", x);
//}
//
//void exit_sighandler(int x) {
//    static int sigcount = 0;
//
//    ++sigcount;
//    if (sigcount == 1) {
//        printf("alsaplayer interrupted by signal %d", x);
//        exit(1);
//    }
//    if (sigcount > 5) {
//        kill(getpid(), SIGKILL);
//    }
//}
