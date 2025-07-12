#include "ui.h"
#include "songlist.h"
#include "statusbar.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

ElementList *create_element_list() {
    ElementList *list = malloc(sizeof(ElementList));

    list->size = 0;
    list->capacity = 8;
    list->list = calloc(list->capacity, sizeof(void *));

    return list;
}

void push_element(ElementList *list, void *element) {
    if (list->size >= list->capacity) {
        list->capacity *= 2;
        Element **tmp = (Element **)realloc(list->list, sizeof(void *) * list->capacity);
        memcpy(tmp, list->list, list->size * sizeof(void *));
        free(list->list);
        list->list = tmp;
    }

    list->list[list->size] = element;
    list->size++;
}

UI *create_ui(Window *window) {
    UI *ui = malloc(sizeof(UI));

    ui->window = window;
    ui->elements = create_element_list();

    return ui;
}

void update_ui(UIState *state, UI *ui) {
    for (unsigned int i = 0; i < ui->elements->size; i++) {
        update_element(state, ui->elements->list[i]);
    }
}

void draw_ui(UI *ui) {
    SDL_SetRenderDrawColor(ui->window->renderer, 40, 42, 54, 255);
    SDL_RenderClear(ui->window->renderer);

    for (unsigned int i = 0; i < ui->elements->size; i++) {
        draw_element(ui->elements->list[i], ui);
    }
}

void update_element(UIState *state, Element *element) {
    if (element->frozen) return;
    switch (element->type) {
        case ELEMENT_BOX: update_box(state, element->data.box); break;
        // case ELEMENT_TEXT: break;
        case ELEMENT_IMAGE: update_image(state, element->data.image); break;
        // case ELEMENT_BUTTON: break;
        case ELEMENT_SONGLIST: update_songlist(state, element->data.songlist); break;
        case ELEMENT_STATUSBAR: update_statusbar(state, element->data.status_bar); break;
        default: fprintf(stderr, "couldn't find a method to UPDATE element type: %d\n", element->type); break;
    }
}

void draw_element(Element *element, UI *ui) {
    if (element->invisible) return;
    switch (element->type) {
        case ELEMENT_BOX: {
                              Box *box = element->data.box;
                              SDL_Rect bounds = { .x = box->x, .y = box->y, .w = box->width, .h = box->height };
                              SDL_SetRenderDrawColor(ui->window->renderer, box->color >> 24 & 0xff, box->color >> 16 & 0xff, box->color >> 8 & 0xff, box->color & 0xff);
                              SDL_RenderFillRect(ui->window->renderer, &bounds);
                              break;
                          }
        case ELEMENT_BUTTON: {
                                 Button *button = element->data.button;
                                 SDL_Rect bounds = { .x = button->x, .y = button->y, .w = button->width, .h = button->height };
                                 SDL_SetRenderDrawColor(ui->window->renderer, button->color >> 24 & 0xff, button->color >> 16 & 0xff, button->color >> 8 & 0xff, button->color & 0xff);
                                 SDL_RenderFillRect(ui->window->renderer, &bounds);
                                 break;
                             }
        case ELEMENT_IMAGE: {
                                Image *image = element->data.image;
                                // printf("drawing image: %d x %d @ (%d, %d) -> %p\n", image->width, image->height, image->x, image->y, ui->window->renderer);
                                SDL_Rect rect = (SDL_Rect){ .x = image->x, .y = image->y, .w = image->width, .h = image->height };
                                // SDL_Rect slice = (SDL_Rect){ .x = 0, .y = image->ih / 8, .w = image->iw, .h = image->ih * 3 / 4 };
                                SDL_RenderCopy(ui->window->renderer, image->image, NULL, &rect);
                                break;
                            }
        case ELEMENT_SONGLIST: {
                                   SongList *songlist = element->data.songlist;

                                   const unsigned int real_song_size = songlist->song_size + songlist->margin;

                                   const unsigned int minSong = (int)floorf((float)songlist->scrolloff / real_song_size);
                                   unsigned int maxSong = (int)ceilf((float)(songlist->scrolloff + ui->window->height) / real_song_size);
                                   const unsigned int length = songlist->songs.size;
                                   if (maxSong > length) {
                                       maxSong = length;
                                   }

                                   for (unsigned int i = minSong; i < maxSong; i++) {
                                       Song *song = &songlist->songs.songs[i];

                                       // printf("%s\n", songlist->songs.songs[i].name);
                                       if (i == songlist->selected_index) {
                                           SDL_SetRenderDrawColor(ui->window->renderer, 0x62, 0x72, 0xa4, 255);
                                       } else {
                                           SDL_SetRenderDrawColor(ui->window->renderer, 0x44, 0x47, 0x5a, 255);
                                       }
                                       SDL_Rect background_rect = (SDL_Rect){
                                           .x = songlist->x,
                                           .y = songlist->y + i * real_song_size - songlist->scrolloff + songlist->margin, // add "top padding"
                                           .w = songlist->width,
                                           .h = songlist->song_size, // don't add padding in order to get gaps between each track
                                       };
                                       SDL_RenderFillRect(ui->window->renderer, &background_rect);

                                       SDL_Rect slice = (SDL_Rect){ .x = 0, .y = song->ih / 8, .w = song->iw, .h = song->ih * 3 / 4 };
                                       SDL_Rect dest = (SDL_Rect){
                                           .x = background_rect.x,
                                           .y = background_rect.y,
                                           .w = background_rect.h * 16 / 9, // TODO: un-hard-code aspect ratio... maybe?
                                           .h = background_rect.h,
                                       };

                                       SDL_RenderCopy(ui->window->renderer, song->image, &slice, &dest);

                                       const unsigned int inset = dest.w + songlist->padding;
                                       SDL_Rect title_dest = {
                                           .x = background_rect.x + inset,
                                           .y = background_rect.y + (background_rect.h - song->title_surface->h) / 2,
                                           .h = song->title_surface->h,
                                       };
                                       const unsigned int max_title_width = background_rect.w - inset - songlist->padding;
                                       if (song->title_surface->w < max_title_width) {
                                           title_dest.w = song->title_surface->w;
                                       } else {
                                           title_dest.w = max_title_width;
                                       }
                                       SDL_Rect title_slice = {
                                           .x = 0,
                                           .y = 0,
                                           .w = title_dest.w,
                                           .h = song->title_surface->h,
                                       };
                                       SDL_RenderCopy(ui->window->renderer, song->title_texture, &title_slice, &title_dest);
                                   }
                                   break;
                               }
        case ELEMENT_STATUSBAR: {
                                    StatusBar *bar = element->data.status_bar;
                                    SDL_Rect background_rect = {
                                        .x = bar->margin,
                                        .y = ui->window->height - (bar->size + bar->margin),
                                        .w = ui->window->width - 2 * bar->margin,
                                        .h = bar->size,
                                    };
                                    bar->x = background_rect.x;
                                    bar->y = background_rect.y;
                                    bar->width = background_rect.w;
                                    bar->height = background_rect.h;
                                    SDL_SetRenderDrawColor(ui->window->renderer, 0x44, 0x47, 0x5a, 255);
                                    SDL_RenderFillRect(ui->window->renderer, &background_rect);

                                    const unsigned int progress_bar_thickness = 6;
                                    const unsigned int progress_bar_x = (background_rect.w - bar->progress_bar_length) / 2;
                                    const unsigned int progress_bar_y = background_rect.y + (background_rect.h - progress_bar_thickness) / 2;

                                    SDL_Rect bar_rect = {
                                        progress_bar_x,
                                        progress_bar_y,
                                        bar->progress_bar_length,
                                        progress_bar_thickness
                                    };
                                    bar->playbar_x = bar_rect.x;
                                    bar->playbar_y = bar_rect.y;
                                    bar->playbar_width = bar_rect.w;
                                    bar->playbar_height = bar_rect.h;
                                    SDL_SetRenderDrawColor(ui->window->renderer, 0x62, 0x72, 0xa4, 255);
                                    SDL_RenderFillRect(ui->window->renderer, &bar_rect);

                                    bar_rect.w *= bar->song_progress / bar->song_length;
                                    SDL_SetRenderDrawColor(ui->window->renderer, 0xf8, 0xf8, 0xf2, 255);
                                    SDL_RenderFillRect(ui->window->renderer, &bar_rect);

                                    const unsigned int volume_bar_thickness = 6;
                                    const unsigned int volume_bar_x = ui->window->width - bar->volume_bar_length - bar->margin - bar->padding;
                                    const unsigned int volume_bar_y = background_rect.y + (background_rect.h - volume_bar_thickness) / 2;

                                    SDL_Rect volume_bar_rect = {
                                        volume_bar_x,
                                        volume_bar_y,
                                        bar->volume_bar_length,
                                        volume_bar_thickness
                                    };
                                    bar->volume_x = volume_bar_rect.x;
                                    bar->volume_y = volume_bar_rect.y;
                                    bar->volume_width = volume_bar_rect.w;
                                    bar->volume_height = volume_bar_rect.h;
                                    SDL_SetRenderDrawColor(ui->window->renderer, 0x62, 0x72, 0xa4, 255);
                                    SDL_RenderFillRect(ui->window->renderer, &volume_bar_rect);

                                    volume_bar_rect.w *= (float)bar->song_volume / MIX_MAX_VOLUME;
                                    SDL_SetRenderDrawColor(ui->window->renderer, 0xf8, 0xf8, 0xf2, 255);
                                    SDL_RenderFillRect(ui->window->renderer, &volume_bar_rect);

                                    const char *title = bar->song_name;
                                    printf("TITLE: %s\n", title);
                                    SDL_Rect title_loc = {
                                        .x = bar->x + bar->padding,
                                        .y = bar->y + bar->padding,
                                    };
                                    title_loc.w = progress_bar_x - bar->padding - title_loc.x;
                                    title_loc.h = bar->height - 2 * bar->padding;

                                    SDL_SetRenderDrawColor(ui->window->renderer, 100, 100, 100, 255);
                                    SDL_RenderFillRect(ui->window->renderer, &title_loc);

                                    break;
                                }
        default: fprintf(stderr, "couldn't find a method to DRAW element type: %d\n", element->type); break;
    }
}
