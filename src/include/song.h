#ifndef SONG_H
#define SONG_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>

typedef struct {
    const char *id;
    const char *name;
    SDL_Texture *image;
    int iw, ih;
    SDL_Surface *title_surface;
    SDL_Texture *title_texture;
} Song;

Song song_from_yt(const char *url);
void download_song(Song s);
void download_thumbnail(Song s);

double get_wav_length(const char *filepath);

#endif
