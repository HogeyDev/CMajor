#include "image.h"
#include "state.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>

Image new_image(SDL_Renderer *renderer, unsigned int x, unsigned int y, unsigned int width, unsigned int height, const char *path) {
    Image img = (Image){
        .x = x,
        .y = y,
        .width = width,
        .height = height,
        .image = IMG_LoadTexture(renderer, path),
    };
    SDL_QueryTexture(img.image, NULL, NULL, &img.iw, &img.ih);
    return img;
}

void update_image(UIState *state, Image *image) {
    if (
            state->mouse->x >= image->x &&
            state->mouse->x < image->x + image->width &&
            state->mouse->y >= image->y &&
            state->mouse->y < image->y + image->height
       ) {
        if (state->mouse->state & LMB_MASK << DOWN_EDGE_SHIFT) {
            if (image->onclick != 0) image->onclick(state, image);
        }
    }
}
