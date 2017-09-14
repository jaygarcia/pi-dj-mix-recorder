/* 
 * File:   main.c
 * Author: jgarcia
 *
 * Created on September 13, 2017, 9:34 AM
 */

#include <stdio.h>
#include <stdlib.h>                     
#include <gtk/gtk.h>
#include "AudioPlayer/AudioPlayer.h"

#include <pthread.h>
#include <unistd.h>

#include "APlay/APlay.h"

void setupSignalHandlers();
void nonfatal_sighandler();
void exit_sighandler();

int main(int argc, char** argv) {

    GtkWidget *window;
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);
    gtk_window_set_title(GTK_WINDOW(window), "TEST 123");
    gtk_widget_show(window);

    g_signal_connect(window, "destroy",  G_CALLBACK(gtk_main_quit), NULL);
        
    gtk_main();

    //This is just for testing.
    audioAction("/home/jgarcia/projects/djsr/song.wav", "play");
    
    return (EXIT_SUCCESS);
}
void setupSignalHandlers() {
    // First setup signal handler
    signal(SIGPIPE, nonfatal_sighandler); // PIPE (socket control)
    signal(SIGTERM, exit_sighandler); // kill
    signal(SIGHUP,  exit_sighandler); // kill -HUP / xterm closed
    signal(SIGINT,  exit_sighandler); // Interrupt from keyboard
    signal(SIGQUIT, exit_sighandler); // Quit from keyboard
    // fatal errors
    signal(SIGBUS,  exit_sighandler); // bus error
    //signal(SIGSEGV, exit_sighandler);	// segfault
    signal(SIGILL,  exit_sighandler); // illegal instruction
    signal(SIGFPE,  exit_sighandler); // floating point exc.
    signal(SIGABRT, exit_sighandler); // abort()
}


void nonfatal_sighandler(int x) {
    printf("Warning: alsaplayer interrupted by signal %d\n", x);
}

void exit_sighandler(int x) {
    static int sigcount = 0;

    ++sigcount;
    if (sigcount == 1) {
        printf("alsaplayer interrupted by signal %d", x);
        exit(1);
    }
    if (sigcount > 5) {
        kill(getpid(), SIGKILL);
    }
}
