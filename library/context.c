#include <hemp/context.h>



hemp_context_t
hemp_context_init() {
    hemp_context_t context = (hemp_context_t) hemp_mem_alloc(
        sizeof(struct hemp_context_s)
    );
    
    if (! context)
        hemp_mem_fail("context");

    return context;
}


void
hemp_context_free(
    hemp_context_t context
) {
    hemp_debug_mem("Releasing context at %p\n", context);
    hemp_mem_free(context);
}
