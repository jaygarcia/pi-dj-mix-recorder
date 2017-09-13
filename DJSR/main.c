/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: jgarcia
 *
 * Created on September 13, 2017, 9:34 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

/* 
 * simple gtk application 
 * author jan bodnar
 * date february 17, 2008
 */

int main(int argc, char** argv) {

    GtkWidget *window;
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);
    gtk_window_set_title(GTK_WINDOW(window), "gtkapp");
    gtk_widget_show(window);

    g_signal_connect(window, "destroy",
		    G_CALLBACK (gtk_main_quit), NULL);
 
    gtk_main();

    return (EXIT_SUCCESS);
}