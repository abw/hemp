#include <hemp/fragments.h>


hemp_fragments
hemp_fragments_new(
    hemp_hemp   hemp,
    hemp_size   capacity
) {
    hemp_fragments fragments;
    HEMP_ALLOCATE(fragments);

    if (! capacity)
        capacity = HEMP_FRAGMENTS_SIZE;
    
    fragments->pool = hemp_pool_new(
        sizeof(struct hemp_fragment), 
        capacity,
        NULL
    );
    fragments->pool->cleaner = &hemp_fragments_free_fragment;

    fragments->hemp = hemp;
    fragments->head = NULL;
    fragments->tail = NULL;

    return fragments;
}


void
hemp_fragments_free(
    hemp_fragments fragments
) {
    hemp_pool_free(fragments->pool);
    hemp_mem_free(fragments);
}


hemp_bool
hemp_fragments_free_fragment(
    hemp_memory item
) {
    hemp_fragment f = (hemp_fragment) item;
    hemp_fragment_cleanup(f);
    
    return HEMP_TRUE;
}


hemp_fragment
hemp_fragments_new_fragment(
    hemp_fragments  fragments,
    hemp_element    type,
    hemp_string     token,
    hemp_pos        position,
    hemp_size       length
) {
    hemp_fragment fragment = (hemp_fragment) hemp_pool_take(
        fragments->pool
    );
    
    fragment->fragments  = fragments;
    fragment->type       = type;
    fragment->token      = token;
    fragment->position   = position;
    fragment->length     = length;
    fragment->flags      = type->flags;
    fragment->next       = NULL;

//  hemp_debug("created new %s fragment at %p\n", type->name, fragment);

    return fragment;
}


hemp_fragment
hemp_fragments_add_fragment(
    hemp_fragments  fragments,
    hemp_element    type,
    hemp_string     token,
    hemp_pos        position,
    hemp_size       length
) {
    hemp_fragment fragment = hemp_fragments_new_fragment(
        fragments, type, token, position, length
    );

//  hemp_fragment_debug(fragment);
    
    if (fragments->tail) {
        fragments->tail->next = fragment;
    }
    else {
        fragments->head = fragments->tail = fragment;
    }
    fragments->tail = fragment;
    fragment->next  = NULL;

    return fragment;
}


hemp_fragment
hemp_fragments_add_eof(
    hemp_fragments  fragments,
    hemp_pos        position
) {
    return hemp_fragments_add_fragment(
        fragments, HempElementEOF,
        NULL, position, 0
    );
}


void 
hemp_fragments_dump(
    hemp_fragments   fragments
) {
    hemp_debug("\n-- frags --\n");
    hemp_fragment f = fragments->head;

    while (f) {
        hemp_fragment_debug(f);
        f = f->next;
    }
    hemp_debug("-- /frags --\n");
}
