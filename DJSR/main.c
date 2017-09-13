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


int main(int argc, char** argv) {

    GtkWidget *window;
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);
    gtk_window_set_title(GTK_WINDOW(window), "TEST 123");
    gtk_widget_show(window);

    g_signal_connect(window, "destroy",  G_CALLBACK (gtk_main_quit), NULL);
    
    playAudioTest("/home/jgarcia/djsr/song.wav");
    
    gtk_main();

    return (EXIT_SUCCESS);
}