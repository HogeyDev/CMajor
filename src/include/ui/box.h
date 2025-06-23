#ifndef BOX_H
#define BOX_H

#include "state.h"

typedef struct Box {
    unsigned int x;
    unsigned int y;

    unsigned int width;
    unsigned int height;

    int color;
    void (*onclick)(UIState *, struct Box *);
} Box;

Box new_box(unsigned int x, unsigned int y, unsigned int width, unsigned int height, int color);
void update_box(UIState *state, Box *box);

#endif
