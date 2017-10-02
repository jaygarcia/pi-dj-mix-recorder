#include "app.h"

#include "../pa/PaPlay.h"
#include <sys/time.h>

#ifdef __arm__
char *TEST_SONG_TO_PLAY = "/home/pi/projects/djsr/song.wav";
#else
char *TEST_SONG_TO_PLAY = "/home/jgarcia/projects/djsr/song.wav";
#endif

void * playAudioThreadFn(gpointer data) {
    playTrack();
}


void * displayTimestampThreadFn(gpointer data) {
    short status = 1;

    double timestamp;
    while(status == 1)  {

        timestamp = getCurrentTimeStamp();
        printf("TIMESTAMP :: %lf\n", timestamp);
        fflush(stdout);

        sleep(1);
        status = getPlaybackStatus();

    }

    printf("EXITING THREAD!!!\n");
}

void onPlayPress(GtkWidget *button, gpointer data) {
    puts("Pressed play!***");
    fflush(stdout);

    GThread *thread = g_thread_new("audio_player", &playAudioThreadFn, NULL);
    g_thread_unref(thread);


    GThread *tsThread = g_thread_new("audio_timestamp", &displayTimestampThreadFn, NULL);
    g_thread_unref(tsThread);
}

void onStopPress(GtkWidget *stopButton, gpointer data) {
    puts("*****Pressed STOP!****\n");
    fflush(stdout);
    stopAudio();
}

void * openThreadFn( gpointer data) {
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    openFile(TEST_SONG_TO_PLAY);

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    u_int64_t delta = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec - start.tv_nsec) / 1000;

    printf("Time to open file %ld\n", delta);
}

void onOpenPress(GtkWidget *openButton, gpointer data) {
    puts("Pressed OPEN");
    fflush(stdout);


    GThread *thread = g_thread_new("audio_opener", &openThreadFn, NULL);
    g_thread_unref(thread);
}


void * pauseAudioThreadFn(gpointer data) {
    pauseAudio();
}

void onPausePress(GtkWidget *pauseButton, gpointer data) {
    puts("Pressed Pause");
    fflush(stdout);

    GThread *thread = g_thread_new("audio_pauser", &pauseAudioThreadFn, NULL);
    g_thread_unref(thread);
}

void layoutUI(GtkWidget *window) {
    
    GtkWidget *button_box  = gtk_button_box_new(GTK_ORIENTATION_VERTICAL),
              *dmmyButton  = gtk_button_new_with_label(" "),
              *openButton  = gtk_button_new_with_label("Open"),
              *playButton  = gtk_button_new_with_label("Play"),
              *pauseButton = gtk_button_new_with_label("Pause"),
              *stopButton  = gtk_button_new_with_label("STOP");


    g_signal_connect(playButton, "pressed", G_CALLBACK(onPlayPress), NULL);
    g_signal_connect(stopButton, "pressed", G_CALLBACK(onStopPress), NULL);
    g_signal_connect(openButton, "pressed", G_CALLBACK(onOpenPress), NULL);
    g_signal_connect(pauseButton, "pressed", G_CALLBACK(onPausePress), NULL);


    gtk_container_add(GTK_CONTAINER(button_box), dmmyButton);
    gtk_container_add(GTK_CONTAINER(button_box), openButton);
    gtk_container_add(GTK_CONTAINER(button_box), playButton);
    gtk_container_add(GTK_CONTAINER(button_box), pauseButton);
    gtk_container_add(GTK_CONTAINER(button_box), stopButton);

    gtk_container_add(GTK_CONTAINER(window), button_box);

    
}

// Follows https://developer.gnome.org/gtk3/stable/gtk-getting-started.html#id-1.2.3.5
void onGtkActivate(GtkApplication* app, gpointer user_data) {
    
//    GtkWidget *window = gtk_application_window_new(app);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_application(GTK_WINDOW(window), app);


    gtk_window_set_title(GTK_WINDOW(window), "DJ Mix Recorder");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 150);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    layoutUI(window);
    gtk_widget_show_all(window);


    gtk_window_activate_focus(GTK_WINDOW(window));
    gtk_window_set_focus_visible(window, 1);


    
    g_signal_connect(window, "destroy",  G_CALLBACK(onWindowDestroy), NULL);

    onOpenPress(NULL, NULL);
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

