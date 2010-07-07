#ifndef HEMP_ELEMENT_H
#define HEMP_ELEMENT_H

#include "hemp/memory.h"

typedef struct hemp_etype   * hemp_etype_t;
typedef struct hemp_element * hemp_element_t;

struct hemp_etype {
    hemp_ident_t    name;
    hemp_text_t     (*text)();
};

struct hemp_element {
    hemp_etype_t    type;
    hemp_text_t     token;
    hemp_pos_t      position;
    hemp_size_t     length;
    hemp_element_t  next;
};

hemp_element_t  hemp_element_new(
    hemp_etype_t    type, 
    hemp_text_t     token,
    hemp_pos_t      position,
    hemp_size_t     length
);

hemp_element_t  hemp_element_init();
void            hemp_element_free(hemp_element_t element);

#endif /* HEMP_ELEMENT_H */