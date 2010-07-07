#ifndef HEMP_ELEMENTS_H
#define HEMP_ELEMENTS_H

#include "hemp/pool.h"
#include "hemp/element.h"

#define HEMP_DEFAULT_ELEMENTS_SIZE  32

typedef struct hemp_elements * hemp_elements_t;

struct hemp_elements {
    hemp_pool_t     pool;
    hemp_element_t  head;
    hemp_element_t  tail;
};

hemp_elements_t
    hemp_elements_init(hemp_size_t capacity);

hemp_element_t
    hemp_elements_append(
        hemp_elements_t elements,
        hemp_etype_t    type,
        hemp_text_t     token,
        hemp_pos_t      position,
        hemp_size_t     length
    );

hemp_element_t
    hemp_elements_eof(
        hemp_elements_t elements,
        hemp_text_t     token,
        hemp_pos_t      position
    );

void hemp_elements_free(hemp_elements_t elements);
void hemp_elements_dump(hemp_elements_t elements);


extern hemp_etype_t HempElementBase;
extern hemp_etype_t HempElementSpace;
extern hemp_etype_t HempElementText;
extern hemp_etype_t HempElementTagStart;
extern hemp_etype_t HempElementTagEnd;
extern hemp_etype_t HempElementEof;


#endif /* HEMP_ELEMENTS_H */
