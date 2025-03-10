#ifndef SONG_H
#define SONG_H

typedef struct {
    const char *url;
} Song;

Song song_from_yt(const char *url);
void download_song(Song s);

#endif
