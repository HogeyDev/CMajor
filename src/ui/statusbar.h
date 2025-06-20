#ifndef STATUSBAR_H
#define STATUSBAR_H

#include "state.h"
#include <SDL2/SDL_ttf.h>

typedef struct {
    char *song_name;
    double song_progress;
    double song_length;

    unsigned int progress_bar_length;
    TTF_Font *playtime_font;

    unsigned int padding;
    unsigned int margin;
    unsigned int size;

    unsigned int x, y;
    unsigned int width, height;

    unsigned int playbar_x, playbar_y;
    unsigned int playbar_width, playbar_height;
} StatusBar;

extern StatusBar *status_bar;

void update_statusbar(UIState *state, StatusBar *bar);

#endif
