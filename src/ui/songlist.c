#include "songlist.h"
#include "song.h"
#include "state.h"
#include "statusbar.h"
#include <SDL2/SDL_mixer.h>
#include <stdio.h>

void update_songlist(UIState *state, SongList *songlist) {
    if (
            state->mouse->x >= songlist->x &&
            state->mouse->x < songlist->x + songlist->width &&
            state->mouse->y >= songlist->y &&
            state->mouse->y < songlist->y + songlist->height
       ) {
        if (state->mouse->scroll) {
            songlist->scrolloff -= state->mouse->scroll * 64;
            const unsigned int songlist_height = (songlist->song_size + songlist->margin) * songlist->songs.size;
            if (songlist_height < songlist->height) {
                songlist->scrolloff = 0;
            } else {
                if (songlist->scrolloff <= 0) {
                    songlist->scrolloff = 0;
                }
                if (songlist_height - songlist->scrolloff <= songlist->height) {
                    songlist->scrolloff = songlist_height - songlist->height;
                }
            }
        }
        if (state->mouse->state & LMB_MASK << DOWN_EDGE_SHIFT) {
            const int inner_y = state->mouse->y - songlist->y + songlist->scrolloff;
            const int track = (int)floorf((float)inner_y / (songlist->song_size + songlist->margin));
            if (track < songlist->songs.size) {
                if (track == songlist->selected_index) {
                    printf("playing track %d: %s\n", track, songlist->songs.songs[track].name);
                    songlist->selected_index = -1;

                    char song_path[128];
                    sprintf(song_path, "songs/%s.wav", songlist->songs.songs[track].id);
                    Mix_Music *music = Mix_LoadMUS(song_path);
                    if (!music) {
                        fprintf(stderr, "Could not load song\n"); 
                    }

                    status_bar->song_length = get_wav_length(song_path);
                    Mix_VolumeMusic(status_bar->song_volume);
                    Mix_PlayMusic(music, 0);
                } else {
                    printf("selected track %d: %s\n", track, songlist->songs.songs[track].name);

                    songlist->selected_index = track;
                }
            }
        }
    }
}
