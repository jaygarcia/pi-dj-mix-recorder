#include "app.h"




#include "../APlay/APlay.h"


void playAudio(gpointer data) {
  
    
    audioAction("/home/jgarcia/projects/djsr/song.wav", "play");

}

void onPlayPress(GtkWidget *button, gpointer data) {
    g_print("Pressed play!");
    GThread *thread;
    
    thread = g_thread_new("audio_player", playAudio, NULL);
    
    g_thread_unref(thread);
}


void layoutUI(GtkWidget *window) {
    
    GtkWidget *button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL),
              *button     = gtk_button_new_with_label("Play");
    
    g_signal_connect(button, "clicked", G_CALLBACK(onPlayPress), NULL);
    
    gtk_container_add(GTK_CONTAINER(window), button_box);
    gtk_container_add(GTK_CONTAINER(button_box), button);
    
    
}

// Follows https://developer.gnome.org/gtk3/stable/gtk-getting-started.html#id-1.2.3.5
void onGtkActivate(GtkApplication* app, gpointer user_data) {
    
    GtkWidget *window = gtk_application_window_new(app);
    
    gtk_window_set_title(GTK_WINDOW(window), "DJ Mix Recorder");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    layoutUI(window);

    
    gtk_widget_show_all(window);
    
    g_signal_connect(window, "destroy",  G_CALLBACK(gtk_main_quit), NULL);    
}


void appStart() {
    GtkApplication* app = gtk_application_new("com.moduscreate.djmr", G_APPLICATION_FLAGS_NONE);
    
    g_signal_connect(app, "activate", G_CALLBACK(onGtkActivate), NULL);
    
    int status = g_application_run(G_APPLICATION(app), 0, NULL);
    
    g_object_unref(app);
}

