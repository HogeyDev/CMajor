#ifndef STATE_H
#define STATE_H

#include "window.h"

#define LMB_MASK 0b01
#define RMB_MASK 0b10
#define DOWN_EDGE_SHIFT 2
#define UP_EDGE_SHIFT 4

typedef struct {
    int x;
    int y;

    int anchor_x; // where is the mouse clicking from? (esp when dragging)
    int anchor_y;

    unsigned char state; // __ 00 11 22
                         // 0 -> up edge,       ordered [rmb, lmb]
                         // 1 -> down edge,     ordered [rmb, lmb]
                         // 2 -> raw buttons,   ordered [rmb, lmb]

    int scroll;
} Mouse;

typedef struct {
    // uhh, idk yet.
} Keyboard;

typedef struct {
    Window *window;
    Mouse *mouse;
    Keyboard *keyboard;
    unsigned char volume;
} UIState;

void update_mouse_state(Mouse *mouse);
void clear_mouse_edges(Mouse *mouse);

#endif
