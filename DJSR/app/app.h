#include <stdio.h>
#include <stdlib.h>                     
#include <unistd.h>

#include <pthread.h>
#include <gtk/gtk.h>



#ifndef APP_H
#define APP_H

#ifdef __cplusplus
extern "C" {
#endif
    
void layoutUI(GtkWidget *window);
   
void onGtkActivate();
void appStart();


#ifdef __cplusplus
}
#endif

#endif /* APP_H */


