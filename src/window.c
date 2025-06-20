#include "window.h"
#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>

Window create_window(unsigned int width, unsigned int height, const char *title) {
    SDL_Window *w = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    SDL_Renderer *r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);

    Window window = { 0 };
    window.window = w;
    window.renderer = r;

    window.width = width;
    window.height = height;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); // enable bilinear mode

    return window;
}

void draw_rectangle(Window *window, int x, int y, unsigned int width, unsigned int height) {
    SDL_Rect r = {
        .x = x, .y = y,
        .w = width, .h = height,
    };

    SDL_RenderDrawRect(window->renderer, &r);
}
void fill_rectangle(Window *window, int x, int y, unsigned int width, unsigned int height) {
    SDL_Rect r = {
        .x = x, .y = y,
        .w = width, .h = height,
    };

    SDL_RenderFillRect(window->renderer, &r);
}

void draw_playlist(Window *window, Playlist *playlist) {
    static const unsigned int visible_rows = 4;

    for (unsigned int i = 0; i < playlist->size; i++) {
        char path[512];
        snprintf(path, 512, "songs/%s.jpg", playlist->songs[i].id);

        SDL_Texture *img = IMG_LoadTexture(window->renderer, path);
        SDL_SetTextureScaleMode(img, SDL_ScaleModeLinear);
        int w, h;
        SDL_QueryTexture(img, NULL, NULL, &w, &h);

        const float ratio = (float)w / (float)h * 4.0f / 3.0f; // adjust for clipped space. this should return 16:9, since the original is 4:3
        // const float ratio = 16.0f / 9.0f;

        SDL_Rect rect;
        rect.h = window->height / visible_rows * 3 / 4;
        rect.w = rect.h * ratio;
        rect.x = (window->width - w) / 2;
        rect.y = rect.h * i;

        SDL_Rect slice;
        slice.x = 0;
        slice.y = h / 8;
        slice.w = w;
        slice.h = h * 3 / 4;

        SDL_RenderCopy(window->renderer, img, &slice, &rect);
        SDL_DestroyTexture(img);
    }
}
