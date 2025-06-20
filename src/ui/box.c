#include "box.h"
#include "state.h"

Box new_box(unsigned int x, unsigned int y, unsigned int width, unsigned int height, int color) {
    return (Box){
        .x = x,
        .y = y,
        .width = width,
        .height = height,
        .color = color,
    };
}

void update_box(UIState *state, Box *box) {
    box->color = 0x00ff00ff;
    if (
            state->mouse->x >= box->x &&
            state->mouse->x < box->x + box->width &&
            state->mouse->y >= box->y &&
            state->mouse->y < box->y + box->height
       ) {
        box->color = 0xff0000ff;
        if (state->mouse->state & LMB_MASK << DOWN_EDGE_SHIFT) {
            if (box->onclick != 0) box->onclick(state, box);
        }
    }
}
