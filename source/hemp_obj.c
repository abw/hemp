#include <stdio.h>
#include "hemp.h"
#include "hemp/debug.h"



hemp_t
hemp_init() {
    hemp_t hemp;

    if ((hemp = (hemp_t) hemp_mem_init(sizeof(struct hemp)))) {
        hemp->dialects = hemp_hash_init();
        if (hemp->dialects) {
            debug_mem("Allocated hemp at %p\n", hemp);
        }
        else {
            hemp_null(hemp);
        }
    }
    
    // TODO: handle hemp == NULL

    return hemp;
}


void
hemp_free(
    hemp_t hemp
) {
    debug_mem("Releasing hemp at %p\n", hemp);

    if (hemp->dialects) {
        hemp_hash_free(hemp->dialects);
    }

    hemp_mem_free(hemp);
}


hemp_bool_t
hemp_register_dialect(
    hemp_t          hemp,
    hemp_cstr_t     name,
    hemp_build_fn   builder
) {
    debug_cyan("registering dialect %s => %p\n", name, builder);
    return HEMP_TRUE;
}
