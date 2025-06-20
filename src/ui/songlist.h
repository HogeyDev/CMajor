#ifndef SONGLIST_H
#define SONGLIST_H

#include "playlist.h"
#include "state.h"
#include <SDL2/SDL_ttf.h>

#define DOUBLE_CLICK_MILLIS 250
typedef struct {
    Playlist songs;
    int x, y;
    int width, height;

    int song_size;
    int scrolloff;
    unsigned int margin;
    unsigned int padding;

    int selected_index;
    TTF_Font *font;
} SongList;

void update_songlist(UIState *state, SongList *songlist);

#endif
