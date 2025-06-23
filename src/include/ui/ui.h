#ifndef UI_H
#define UI_H

#include "image.h"
#include "songlist.h"
#include "state.h"
#include "box.h"
#include "button.h"
#include "statusbar.h"
#include "window.h"

typedef enum {
    ELEMENT_BOX,
    ELEMENT_BUTTON,
    ELEMENT_IMAGE,
    ELEMENT_SONGLIST,
    ELEMENT_STATUSBAR,
    ELEMENT_TEXT,
} ElementType;

typedef struct {
    unsigned char invisible; // not rendered
    unsigned char frozen; // not interactable
    ElementType type;
    union {
        Box *box;
        Image *image;
        Button *button;
        SongList *songlist;
        StatusBar *status_bar;
    } data;
} Element;

typedef struct {
    unsigned int size;
    unsigned int capacity;
    Element **list;
} ElementList;

ElementList *create_element_list();
void push_element(ElementList *list, void *element);

typedef struct UI {
    Window *window;
    ElementList *elements;
} UI;

UI *create_ui(Window *window);
void update_ui(UIState *state, UI *ui);
void draw_ui(UI *ui);

void update_element(UIState *state, Element *element);
void draw_element(Element *element, UI *ui);

#endif
