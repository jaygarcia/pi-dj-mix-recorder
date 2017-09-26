#include "app.h"

#include "../pa/PaPlay.h"
#include <sys/time.h>

void * playAudio(gpointer data) {
    playTrack();
}

void onPlayPress(GtkWidget *button, gpointer data) {
    puts("Pressed play!***");
    fflush(stdout);

    GThread *thread = g_thread_new("audio_player", &playAudio, NULL);

    g_thread_unref(thread);
}

void onStopPress(GtkWidget *stopButton, gpointer data) {
    puts("*****Pressed STOP!****\n");
    fflush(stdout);
    stopAudio();
}


void * openAFile(gpointer data) {

}

void onOpenPress(GtkWidget *openButton, gpointer data) {
    puts("Open pressed");
    fflush(stdout);

    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    openFile("/home/jgarcia/projects/djsr/song.wav");

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    u_int64_t delta = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;

    printf("Time to open file %ld\n", delta);
}


void layoutUI(GtkWidget *window) {
    
    GtkWidget *button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL),
              *dmmyButton = gtk_button_new_with_label("WTF"),
              *openButton = gtk_button_new_with_label("Open"),
              *playButton = gtk_button_new_with_label("Play"),
              *stopButton = gtk_button_new_with_label("STOP");


    g_signal_connect(playButton, "pressed", G_CALLBACK(onPlayPress), NULL);
    g_signal_connect(stopButton, "pressed", G_CALLBACK(onStopPress), NULL);
    g_signal_connect(openButton, "pressed", G_CALLBACK(onOpenPress), NULL);


    gtk_container_add(GTK_CONTAINER(button_box), dmmyButton);
    gtk_container_add(GTK_CONTAINER(button_box), openButton);
    gtk_container_add(GTK_CONTAINER(button_box), playButton);
    gtk_container_add(GTK_CONTAINER(button_box), stopButton);

    gtk_container_add(GTK_CONTAINER(window), button_box);

    
}

// Follows https://developer.gnome.org/gtk3/stable/gtk-getting-started.html#id-1.2.3.5
void onGtkActivate(GtkApplication* app, gpointer user_data) {
    
//    GtkWidget *window = gtk_application_window_new(app);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_application(window, app);


    gtk_window_set_title(GTK_WINDOW(window), "DJ Mix Recorder");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 150);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    layoutUI(window);
    gtk_widget_show_all(window);


    gtk_window_activate_focus(GTK_WINDOW(window));
    gtk_window_set_focus_visible(window, 1);


    
    g_signal_connect(window, "destroy",  G_CALLBACK(onWindowDestroy), NULL);
}

void onWindowDestroy() {
    stopAudio();
    cleanup();
    gtk_main_quit();
}


void appStart() {
    GtkApplication* app = gtk_application_new("com.moduscreate.djmixrecorder", G_APPLICATION_FLAGS_NONE);
    
    g_signal_connect(app, "activate", G_CALLBACK(onGtkActivate), NULL);
    
    int status = g_application_run(G_APPLICATION(app), 0, NULL);
    g_application_activate(app);
    
    g_object_unref(app);
}

