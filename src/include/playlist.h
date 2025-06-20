#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "song.h"
typedef struct {
    Song *songs;
    unsigned int size;
    unsigned int cap;
} Playlist;

Playlist load_list(const char *path);
void add_song(Playlist *p, Song s);
void download_list(Playlist *p);

#endif
