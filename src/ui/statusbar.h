#ifndef STATUSBAR_H
#define STATUSBAR_H

#include "state.h"
#include <SDL2/SDL_ttf.h>

typedef struct {
    char *song_name;
    unsigned int song_progress;
    unsigned int song_length;

    unsigned int progress_bar_length;
    TTF_Font *playtime_font;

    unsigned int margin;
    unsigned int size;
} StatusBar;

void update_statusbar(UIState *state, StatusBar *bar);

#endif
