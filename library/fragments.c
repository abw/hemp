#include <hemp/fragments.h>


HempFragments
hemp_fragments_new(
    HempDocument   document,
    HempSize       capacity
) {
    HempFragments  fragments;
    HEMP_ALLOCATE(fragments);

    if (! capacity)
        capacity = HEMP_FRAGMENTS_SIZE;

    fragments->pool = hemp_pool_new(
        sizeof(struct hemp_fragment),
        capacity,
        NULL
    );
    fragments->pool->cleaner = &hemp_fragments_free_fragment;

    fragments->document = document;
    fragments->head     = NULL;
    fragments->tail     = NULL;

    return fragments;
}


void
hemp_fragments_free(
    HempFragments fragments
) {
    hemp_pool_free(fragments->pool);
    hemp_mem_free(fragments);
}


HempBool
hemp_fragments_free_fragment(
    HempMemory item
) {
    HempFragment f = (HempFragment) item;
    hemp_fragment_cleanup(f);

    return HEMP_TRUE;
}


HempFragment
hemp_fragments_new_fragment(
    HempFragments  fragments,
    HempElement    type,
    HempString     token,
    HempPos        position,
    HempSize       length
) {
    HempFragment fragment = (HempFragment) hemp_pool_take(
        fragments->pool
    );

    fragment->fragments  = fragments;
    fragment->type       = type;
    fragment->token      = token;
    fragment->position   = position;
    fragment->length     = length;
    fragment->flags      = type->flags;
    fragment->next       = NULL;
    fragment->branch     = NULL;

//  hemp_debug("created new %s fragment at %p\n", type->name, fragment);

    return fragment;
}


HempFragment
hemp_fragments_add_fragment(
    HempFragments  fragments,
    HempElement    type,
    HempString     token,
    HempPos        position,
    HempSize       length
) {
    hemp_debug_call("hemp_fragments_add_fragment()\n");

    HempFragment fragment = hemp_fragments_new_fragment(
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


HempFragment
hemp_fragments_add_eof(
    HempFragments  fragments,
    HempPos        position
) {
    return hemp_fragments_add_fragment(
        fragments, HempElementEOF,
        NULL, position, 0
    );
}

HempDocument
hemp_fragments_document(
    HempFragments fragments
) {
    if (! fragments->document)
        hemp_fatal("No document defined for fragments");
    return fragments->document;
}


void
hemp_fragments_dump(
    HempFragments   fragments
) {
    hemp_debug("\n-- frags --\n");
    HempFragment f = fragments->head;

    while (f) {
        hemp_fragment_debug(f);
        f = f->next;
    }
    hemp_debug("-- /frags --\n");
}
