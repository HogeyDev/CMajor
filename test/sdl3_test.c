// compile with: gcc -o sdl3_test sdl3_test.c -lSDL3 -lSDL3_ttf

#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_events.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <stdlib.h>

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer("SDL3 Test", 1280, 720, 0, &window, &renderer);

    TTF_Font *font = TTF_OpenFont("/usr/share/fonts/TTF/JetBrainsMonoNerdFont-Regular.ttf", 24);
    TTF_Font *fallback = TTF_OpenFont("../fonts/NotoSansCJKjp-Regular.otf", 24);
    TTF_AddFallbackFont(font, fallback);

    SDL_Event event;
    while (1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                TTF_Quit();
                SDL_Quit();
                exit(0);
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Color white = {
            .r = 255,
            .g = 255,
            .b = 255,
            .a = 255,
        };
        SDL_Surface *text_surface = TTF_RenderText_Blended(font, "my cool text (with jp): 日本語", 0, white);
        SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        SDL_FRect dest = { 100, 100, text_surface->w, text_surface->h };
        SDL_RenderTexture(renderer, text_texture, NULL, &dest);
        SDL_DestroyTexture(text_texture);
        SDL_DestroySurface(text_surface);

        SDL_RenderPresent(renderer);
    }

    TTF_Quit();
    SDL_Quit();
    return 0;
}
