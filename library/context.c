#include <hemp/context.h>


hemp_context_p
hemp_context_init(
    hemp_p hemp
) {
    hemp_context_p context = (hemp_context_p) hemp_mem_alloc(
        sizeof(struct hemp_context_s)
    );
    
    if (! context)
        hemp_mem_fail("context");

    context->hemp = hemp;
    context->vars = hemp_hash_init();

    return context;
}


void
hemp_context_free(
    hemp_context_p context
) {
    // TODO: worry about cleanup
    hemp_hash_free(context->vars);
    hemp_mem_free(context);
}

