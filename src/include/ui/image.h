#ifndef IMAGE_H
#define IMAGE_H

#include <SDL2/SDL_render.h>
#include "state.h"

typedef struct Image {
    unsigned int x;
    unsigned int y;

    unsigned int width;
    unsigned int height;

    SDL_Texture *image;
    int iw, ih;

    void (*onclick)(UIState *, struct Image *);
} Image;

Image new_image(SDL_Renderer *renderer, unsigned int x, unsigned int y, unsigned int width, unsigned int height, const char *path);
void update_image(UIState *state, Image *image);

#endif
