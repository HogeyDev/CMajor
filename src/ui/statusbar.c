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
        }
    }
}
