#ifndef BUTTON_H
#define BUTTON_H

typedef enum {
    Released,
    Clicked,
} ButtonState;

typedef struct {
    unsigned int x;
    unsigned int y;

    unsigned int width;
    unsigned int height;

    int color;

    ButtonState state;
} Button;

#endif
