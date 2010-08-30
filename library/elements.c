#include <hemp/elements.h>


hemp_elements_p
hemp_elements_init(
    hemp_p      hemp,
    hemp_size_t capacity
) {
    hemp_elements_p elements = (hemp_elements_p) hemp_mem_alloc(
        sizeof(struct hemp_elements_s)
    );
    
    if (! elements)
        hemp_mem_fail("elements");

    if (! capacity)
        capacity = HEMP_ELEMENTS_SIZE;
    
    elements->pool = hemp_pool_init(
        sizeof(struct hemp_element_s), 
        capacity
    );

    elements->hemp = hemp;
    elements->head = NULL;
    elements->tail = NULL;

    return elements;
}


void
hemp_elements_free(
    hemp_elements_p elements
) {
    /* call destructor on all elements that define one */
    hemp_element_p e = elements->head;
    hemp_element_p n = e->next;

    while (e) {
        if (e->type->cleanup)
            e->type->cleanup(e);
        e = e->next;
    }

    hemp_pool_free(elements->pool);
    hemp_mem_free(elements);
}


hemp_element_p
hemp_elements_create(
    hemp_elements_p elements,
    hemp_symbol_p   type,
    hemp_str_p      token,
    hemp_pos_t      position,
    hemp_size_t     length
) {
    hemp_element_p element = (hemp_element_p) hemp_pool_take(
        elements->pool
    );
    
    element->type       = type;
    element->token      = token;
    element->position   = position;
    element->length     = length;
    element->flags      = type->flags;
    element->next       = NULL;

//  hemp_debug("created new %s element at %p\n", type->name, element);

    return element;
}


hemp_element_p
hemp_elements_append(
    hemp_elements_p elements,
    hemp_symbol_p   type,
    hemp_str_p      token,
    hemp_pos_t      position,
    hemp_size_t     length
) {
    hemp_element_p element = hemp_elements_create(
        elements, type, token, position, length
    );
    
    if (elements->tail) {
        elements->tail->next = element;
    }
    else {
        elements->head = elements->tail = element;
    }
    elements->tail = element;
    element->next  = NULL;

    return element;
}


hemp_element_p
hemp_elements_eof(
    hemp_elements_p elements,
    hemp_pos_t      position
) {
    return hemp_elements_append(
        elements, HempSymbolEof,
        NULL, position, 0
    );
}


void 
hemp_elements_dump(
    hemp_elements_p elements
) {
    hemp_debug("\n-- Elements --\n");
    hemp_element_p e = elements->head;
    hemp_element_p n = e->next;

    while (e) {
        if (! hemp_element_dump(e) )
            break;
        e = e->next;
    }
}
