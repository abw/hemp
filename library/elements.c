#include <hemp/elements.h>


hemp_elements
hemp_elements_new(
    hemp_hemp   hemp,
    hemp_size   capacity
) {
    hemp_elements elements;
    HEMP_ALLOCATE(elements);

    if (! capacity)
        capacity = HEMP_ELEMENTS_SIZE;
    
    elements->pool = hemp_pool_new(
        sizeof(struct hemp_element), 
        capacity,
        NULL
    );

    elements->hemp = hemp;
    elements->head = NULL;
    elements->tail = NULL;

    return elements;
}


void
hemp_elements_free(
    hemp_elements elements
) {
    // TODO: would it be better to start at first element and walk straight
    // down the memory chain?  That way we don't miss any synthetic elements
    // that might have been created but not bound to the NEXT pointer of an
    // existing element.  Also would avoid problems introduced by adding
    // synthetic elements to EOF->NEXT.

    /* call destructor on all elements that define one */
    hemp_element e = elements->head;
    hemp_element n = e->next;

    while (e) {
        if (e->type->cleanup) {
//          hemp_debug("calling cleanup for %s\n", e->type->name);
            e->type->cleanup(e);
        }
        e = e->next;
    }

    hemp_pool_free(elements->pool);
    hemp_mem_free(elements);
}


hemp_element
hemp_elements_create(
    hemp_elements   elements,
    hemp_symbol     type,
    hemp_string     token,
    hemp_pos        position,
    hemp_size       length
) {
    hemp_element element = (hemp_element) hemp_pool_take(
        elements->pool
    );
    
    element->type       = type;
    element->elements   = elements;
    element->token      = token;
    element->position   = position;
    element->length     = length;
    element->flags      = type->flags;
    element->next       = NULL;

//  hemp_debug("created new %s element at %p\n", type->name, element);

    return element;
}


hemp_element
hemp_elements_append(
    hemp_elements   elements,
    hemp_symbol     type,
    hemp_string     token,
    hemp_pos        position,
    hemp_size       length
) {
    hemp_element element = hemp_elements_create(
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


hemp_element
hemp_elements_eof(
    hemp_elements   elements,
    hemp_pos        position
) {
    return hemp_elements_append(
        elements, HempSymbolEOF,
        NULL, position, 0
    );
}


void 
hemp_elements_dump(
    hemp_elements   elements
) {
    hemp_debug("\n-- Elements --\n");
    hemp_element e = elements->head;
    hemp_element n = e->next;

    while (e) {
        if (! hemp_element_dump(e) )
            break;
        e = e->next;
    }
    hemp_debug("-- /Elements --\n");
}
