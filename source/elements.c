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
            debug_cyan(
                "Allocated pool of %d elements at %p\n", 
                capacity, elements->pool
            );
        }
        else {
            hemp_elements_free(elements);
            elements = NULL;
        }
    }

    // TODO handle elements == NULL
    return elements;
}


void
hemp_elements_free(hemp_elements_t elements) {
    if (elements->pool) {
        hemp_pool_free(elements->pool);
    }
    debug_cyan("Releasing element pool at %p\n", elements->pool);
    hemp_mem_free(elements);
}


hemp_element_t
hemp_elements_append(
    hemp_elements_t elements,
    hemp_etype_t    type,
    hemp_text_t     token,
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
    hemp_text_t     token,
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
    debug("ELEMENTS\n--------\n");
    hemp_element_t e = elements->head;
    hemp_element_t n = e->next;

    while (e) {
        debug("%03d %p %-20s %s%s%s\n", (int) e->position, e, e->type->name, ANSI_YELLOW, e->type->text(e), ANSI_RESET);
        if (e->type == HempElementEof) {
            break;
        }
        e = e->next;
    }
}
    


// TODO: these should be moved to elements/core



hemp_text_t hemp_element_text_text(hemp_element_t element);
hemp_text_t hemp_element_eof_text(hemp_element_t element);


static struct hemp_etype
    hemp_element_base = { 
        "element",
        &hemp_element_text_text
    };

static struct hemp_etype 
    hemp_element_space = { 
        "space",
        &hemp_element_text_text
    };

static struct hemp_etype
    hemp_element_text = { 
        "text",
        &hemp_element_text_text
    };

static struct hemp_etype
    hemp_element_tag_start = { 
        "tag_start",
        &hemp_element_text_text
    };

static struct hemp_etype
    hemp_element_tag_end = { 
        "tag_end",
        &hemp_element_text_text
    };

static struct hemp_etype
    hemp_element_eof = { 
        "eof",
        &hemp_element_eof_text
    };



hemp_etype_t HempElementBase      = &hemp_element_base;
hemp_etype_t HempElementSpace     = &hemp_element_space;
hemp_etype_t HempElementText      = &hemp_element_text;
hemp_etype_t HempElementTagStart  = &hemp_element_tag_start;
hemp_etype_t HempElementTagEnd    = &hemp_element_tag_end;
hemp_etype_t HempElementEof       = &hemp_element_eof;


hemp_text_t
hemp_element_text_text(hemp_element_t element) {
    static char buffer[1000];   // quick hack
    strncpy(buffer, (char *) element->token, element->length); 
    buffer[element->length] = '\0';
    return buffer;
}


hemp_text_t
hemp_element_eof_text(hemp_element_t element) {
    static char nullstr = '\0';
    return &nullstr;
}

