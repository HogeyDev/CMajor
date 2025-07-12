#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/playlist.h"
#include "page.h"
#include "ui/songlist.h"
#include "ui/state.h"
#include "ui/statusbar.h"
#include "ui/ui.h"
#include "window.h"
#include "components.h"
// #include "include/song.h"

#ifdef LIMIT_FRAMERATE
static unsigned int framerate = 240;
#endif

int main(void) {
    status_bar = calloc(1, sizeof(StatusBar));
    Playlist list = load_list("list.txt");
    download_list(&list);
    printf("Cataloging done!\n");

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        fprintf(stderr, "Could not initialize SDL2\n");
        return 1;
    }
    
    if (TTF_Init() < 0) {
        fprintf(stderr, "Could not initialize TTF\n");
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        fprintf(stderr, "Could not initialize SDL2 Mixer\n");
        return 1;
    }

    Window window = create_window(1280, 720, "Nokturn");
    UI *songlist_ui = create_ui(&window);

    SongList songlist = {
        .songs = list,
        .x = 100,
        .y = 0,
        .width = 1080,
        .height = 720,
        .song_size = 32,
        .margin = 8,
        .padding = 8,
        .selected_index = -1,
    };
    // songlist.font = TTF_OpenFont("/usr/share/fonts/TTF/JetBrainsMonoNerdFont-Regular.ttf", songlist.song_size - 2 * songlist.padding);
    TTF_Font *fallback = TTF_OpenFont("fonts/NotoSansCJKjp-Regular.otf", songlist.song_size - 2 * songlist.padding);
    songlist.font = fallback; // until we fully migrate to sdl3, we will just use the fallback font as the main font

    for (unsigned int i = 0; i < list.size; i++) {
        Song *song = &list.songs[i];
        char album_cover_path[128];
        sprintf(album_cover_path, "songs/%s.jpg", song->id);
        song->image = IMG_LoadTexture(window.renderer, album_cover_path);
        SDL_QueryTexture(song->image, NULL, NULL, &song->iw, &song->ih);

        song->title_surface = TTF_RenderUTF8_Blended(songlist.font, song->name, (SDL_Color){ 0xf8, 0xf8, 0xf2, 0xff });
        song->title_texture = SDL_CreateTextureFromSurface(songlist_ui->window->renderer, song->title_surface);
    }

    Element songlist_elem = { 0 };
    songlist_elem.type = ELEMENT_SONGLIST;
    songlist_elem.data.songlist = &songlist;
    push_element(songlist_ui->elements, &songlist_elem);

    status_bar->song_name = calloc(1, sizeof(char));
    status_bar->song_length = 100;
    status_bar->song_progress = 64;
    status_bar->progress_bar_length = window.width / 2;
    status_bar->volume_bar_length = window.width / 8;
    status_bar->margin = 8;
    status_bar->padding = 8;
    status_bar->size = 32;
    status_bar->playtime_font = TTF_OpenFont("fonts/NotoSansCJKjp-Regular.otf", status_bar->size - status_bar->padding * 2);
    Element status_bar_elem = { 0 };
    status_bar_elem.type = ELEMENT_STATUSBAR;
    status_bar_elem.data.status_bar = status_bar;
    push_element(songlist_ui->elements, &status_bar_elem);

    songlist.height -= status_bar->size + status_bar->padding * 2;

    UI *home_ui = create_ui(&window);
    push_element(home_ui->elements, &status_bar_elem);

    UI *settings_ui = create_ui(&window);
    push_element(settings_ui->elements, &status_bar_elem);

    UIState *ui_state = (UIState *)calloc(1, sizeof(UIState));
    ui_state->current_page = PAGE_PLAYLIST;
    ui_state->window = &window;
    ui_state->mouse = (Mouse *)calloc(1, sizeof(Mouse));
    status_bar->song_volume = 4;
    // ui_state->keyboard = (Keyboard *)malloc(sizeof(Keyboard *));

#ifdef FRAMERATE_COUNTER
    Uint32 fps_start_time = SDL_GetTicks();
    int frame_count = 0;
    float fps = 0.0f;
#endif

    SDL_Event event;
    while (1) {
        ui_state->mouse->scroll = 0;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    SDL_Quit();
                    Mix_Quit();
                    exit(0);
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_q && event.key.keysym.mod & SDLK_LCTRL) {
                        SDL_Quit();
                        Mix_Quit();
                        exit(0);
                    }
                    break;
                case SDL_MOUSEMOTION:
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    update_mouse_state(ui_state->mouse);
                    break;
                case SDL_MOUSEWHEEL:
                    ui_state->mouse->scroll = event.wheel.y;
                    break;
            }
        }

        switch (ui_state->current_page) {
                case PAGE_HOME:
                    draw_ui(home_ui);
                    update_ui(ui_state, home_ui);
                    break;
                // case PAGE_LIBRARY: break;
                case PAGE_PLAYLIST:
                    draw_ui(songlist_ui);
                    update_ui(ui_state, songlist_ui);
                    break;
                // case PAGE_QUEUE: break;
                case PAGE_SETTINGS:
                    draw_ui(settings_ui);
                    update_ui(ui_state, settings_ui);
                    break;
                default:
                    printf("Cannot render page: %d\n", ui_state->current_page);
        }

        clear_mouse_edges(ui_state->mouse);
        SDL_RenderPresent(window.renderer);
#ifdef LIMIT_FRAMERATE
        SDL_Delay(1000.0f / framerate);
#endif

#ifdef FRAMERATE_COUNTER
        frame_count++;
        Uint32 current_time = SDL_GetTicks();
        Uint32 elapsed = current_time - fps_start_time;

        if (elapsed >= 1000) {
            fps = frame_count / (elapsed / 1000.0f);
            frame_count = 0;
            fps_start_time = current_time;

            fprintf(stderr, "FPS: %.2f\n", fps);
        }
#endif
    }

    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
    return 0;
}
