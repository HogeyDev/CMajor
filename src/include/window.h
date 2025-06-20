#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include "playlist.h"

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    unsigned int width;
    unsigned int height;
} Window;

Window create_window(unsigned int width, unsigned int height, const char *title);

void draw_rectangle(Window *window, int x, int y, unsigned int width, unsigned int height);
void fill_rectangle(Window *window, int x, int y, unsigned int width, unsigned int height);

void draw_playlist(Window *w, Playlist *p);

#endif
