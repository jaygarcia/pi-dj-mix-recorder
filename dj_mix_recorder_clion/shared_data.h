//
// Created by jgarcia on 9/15/17.
//

#ifndef DJSR_CLION_SHARED_DATA_H
#define DJSR_CLION_SHARED_DATA_H

typedef struct {
    GMutex mutex;
    gboolean isPlaying;
} ThreadBlob;



#endif //DJSR_CLION_SHARED_DATA_H
