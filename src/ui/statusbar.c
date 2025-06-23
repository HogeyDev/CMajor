#include "statusbar.h"
#include "state.h"
#include <SDL2/SDL_mixer.h>

StatusBar *status_bar = NULL;

void update_statusbar(UIState *state, StatusBar *bar) {
    double progress = Mix_GetMusicPosition(NULL);
    if (progress < 0) {
        progress = 0;
    }
    bar->song_progress = progress;

    if (state->mouse->state & LMB_MASK << DOWN_EDGE_SHIFT) {
        if (
            state->mouse->x >= bar->playbar_x &&
            state->mouse->x < bar->playbar_x + bar->playbar_width &&
            state->mouse->y >= bar->playbar_y &&
            state->mouse->y < bar->playbar_y + bar->playbar_height
        ) {
            const unsigned int inset_x = state->mouse->x - bar->playbar_x;
            const double progress = (double)inset_x / bar->playbar_width;
            Mix_SetMusicPosition(progress * bar->song_length);
        } else if (
            state->mouse->x >= bar->volume_x &&
            state->mouse->x < bar->volume_x + bar->volume_width &&
            state->mouse->y >= bar->volume_y &&
            state->mouse->y < bar->volume_y + bar->volume_height
        ) {
            const unsigned int inset_x = state->mouse->x - bar->volume_x;
            const double volume = (double)inset_x / bar->volume_width;
            bar->song_volume = (unsigned char)(volume * MIX_MAX_VOLUME);
            Mix_VolumeMusic(status_bar->song_volume);
        }
    }
}
