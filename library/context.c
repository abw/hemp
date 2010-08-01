#include "hemp/context.h"
#include "hemp/debug.h"



hemp_context_t
hemp_context_init() {
    hemp_context_t context;

    if ((context = (hemp_context_t) hemp_mem_init(sizeof(struct hemp_context)))) {
        debug_mem(
            "Allocated context at %p\n", 
            context
        );
    }
    else {
        hemp_fatal("failed to create context");         // TODO
    }

    return context;
}


void
hemp_context_free(
    hemp_context_t context
) {
    debug_mem("Releasing context at %p\n", context);
    hemp_mem_free(context);
}
