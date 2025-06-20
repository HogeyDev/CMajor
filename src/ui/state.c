#include "state.h"
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_stdinc.h>

void update_mouse_state(Mouse *mouse) {
    // if (SDL_GetMouseFocus() != window) return;

    static unsigned char old_buttons = 0;
    int x, y;
    Uint32 buttons = SDL_GetMouseState(&x, &y);

    mouse->x = x;
    mouse->y = y;

    unsigned char new_buttons = 0;
    if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) new_buttons |= LMB_MASK;
    if (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) new_buttons |= RMB_MASK;

    unsigned char changed = new_buttons ^ old_buttons;
    unsigned char down_edge = changed & new_buttons;
    unsigned char up_edge = changed & old_buttons;

    mouse->state = new_buttons & (LMB_MASK | RMB_MASK)
                 | (down_edge & (LMB_MASK | RMB_MASK)) << DOWN_EDGE_SHIFT
                 | (up_edge & (LMB_MASK | RMB_MASK)) << UP_EDGE_SHIFT;

    old_buttons = new_buttons;
}

void clear_mouse_edges(Mouse *mouse) {
    mouse->state &= ~( (LMB_MASK | RMB_MASK) << DOWN_EDGE_SHIFT );
    mouse->state &= ~( (LMB_MASK | RMB_MASK) << UP_EDGE_SHIFT );
}
