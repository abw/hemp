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
            hemp->verbose = HEMP_FALSE;
            hemp->debug   = HEMP_FALSE;
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


void
hemp_hello() {
    printf("Hello world!\n");
}


hemp_cstr_t
hemp_render(
    hemp_cstr_t    scheme, 
    hemp_cstr_t    source
) {
    hemp_template_t tmpl = hemp_template_init(scheme, source, NULL);

    if (! tmpl) {
        hemp_fatal("could not initialise %s template", scheme);
        return;
    }
    
    debug("got %s %s template\n", tmpl->dialect->name, scheme);
    
    hemp_text_t text = hemp_template_render(tmpl);

    if (! text) {
        hemp_fatal("could not render template output");
        return;
    }
        
    puts(text->string);
    return text->string;                // ...and leak memory
}
