#ifndef STATUSBAR_H
#define STATUSBAR_H

#include "ui/state.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>

typedef struct {
    char *song_name;
    double song_progress;
    double song_length;
    unsigned char song_volume;

    unsigned int progress_bar_length;
    TTF_Font *font;
    SDL_Surface *song_name_surface;
    SDL_Texture *song_name_texture;

    unsigned int padding;
    unsigned int margin;
    unsigned int size;

    unsigned int x, y;
    unsigned int width, height;

    unsigned int playbar_x, playbar_y;
    unsigned int playbar_width, playbar_height;

    unsigned int volume_bar_length;
    unsigned int volume_x, volume_y;
    unsigned int volume_width, volume_height;
} StatusBar;

extern StatusBar *status_bar;

void update_statusbar(UIState *state, StatusBar *bar);

#endif
