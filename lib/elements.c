#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hemp/elements.h"
#include "hemp/debug.h"


hemp_elements_t
hemp_elements_init(hemp_size_t capacity) {
    hemp_elements_t elements;

    if (! capacity)
        capacity = HEMP_DEFAULT_ELEMENTS_SIZE;

    if ((elements = (hemp_elements_t) hemp_mem_init(sizeof(struct hemp_elements)))) {
        if ((elements->pool = hemp_pool_init(sizeof(struct hemp_element), capacity))) {
            elements->head = NULL;
            elements->tail = NULL;
            debug_mem(
                "Allocated pool of %d elements at %p\n", 
                capacity, elements->pool
            );
        }
        else {
            hemp_elements_null(elements);
        }
    }

    // TODO handle elements == NULL
    return elements;
}


void
hemp_elements_free(hemp_elements_t elements) {
    debug_mem("Releasing elements at %p\n", elements);

    if (elements->pool) {
        hemp_pool_free(elements->pool);
    }

    hemp_mem_free(elements);
}


hemp_element_t
hemp_elements_create(
    hemp_elements_t elements,
    hemp_etype_t    type,
    hemp_cstr_t     token,
    hemp_pos_t      position,
    hemp_size_t     length
) {
    hemp_element_t element;
    
    element             = (hemp_element_t) hemp_pool_take(elements->pool);
    element->type       = type;
    element->token      = token;
    element->position   = position;
    element->length     = length;

    return element;
}


hemp_element_t
hemp_elements_append(
    hemp_elements_t elements,
    hemp_etype_t    type,
    hemp_cstr_t     token,
    hemp_pos_t      position,
    hemp_size_t     length
) {
    hemp_element_t element;
    
    element             = (hemp_element_t) hemp_pool_take(elements->pool);
    element->type       = type;
    element->token      = token;
    element->position   = position;
    element->length     = length;

    if (elements->tail) {
        elements->tail->next = element;
    }
    else {
        elements->head = elements->tail = element;
    }
    elements->tail = element;

    return element;
}


hemp_element_t
hemp_elements_eof(
    hemp_elements_t elements,
    hemp_cstr_t     token,
    hemp_pos_t      position
) {
    return hemp_elements_append(
        elements, HempElementEof,
        token, position, 0
    );
}


void 
hemp_elements_dump(
    hemp_elements_t elements
) {
    debug("\n-- Elements --\n");
    hemp_element_t e = elements->head;
    hemp_element_t n = e->next;

    while (e) {
        if (! hemp_element_dump(e) )
            break;
        e = e->next;
    }
}
    

