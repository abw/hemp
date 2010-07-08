#include <stdio.h>
#include "hemp.h"
#include "hemp/debug.h"



hemp_t
hemp_init() {
    hemp_t hemp;

    if ((hemp = (hemp_t) hemp_mem_init(sizeof(struct hemp)))) {
        hemp->dialects = hemp_hash_init();
        if (hemp->dialects) {
            debug_cyan(
                "Allocated new hemp at %p\n", 
                hemp
            );
        }
        else {
            hemp_free(hemp);
            hemp = NULL;
        }
    }
    
    // TODO: handle hemp == NULL

    return hemp;
}


void
hemp_free(
    hemp_t hemp
) {
    if (hemp->dialects) {
        hemp_hash_free(hemp->dialects);
    }
    hemp_mem_free(hemp);
}


