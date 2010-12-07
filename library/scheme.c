#include <hemp/scheme.h>


hemp_scheme
hemp_scheme_new(
    hemp_hemp       hemp,
    hemp_string     name
) {
    hemp_scheme scheme;
    HEMP_ALLOCATE(scheme);

    scheme->hemp    = hemp;
    scheme->name    = hemp_string_clone(name, "scheme name");
    scheme->namer   = &hemp_scheme_namer;
    scheme->checker = NULL;
    scheme->reader  = NULL;
    scheme->cleaner = NULL;

    return scheme;
}


void
hemp_scheme_free(
    hemp_scheme scheme
) {
    if (scheme->name)
        hemp_mem_free(scheme->name);

    hemp_mem_free(scheme);
}


hemp_string 
hemp_scheme_namer(
    hemp_source source
) {
    return source->scheme->name;
}


HEMP_AUTOLOAD(hemp_scheme_autoload) {
    return hemp_use_module(factory->hemp, "scheme", name)
        ? HEMP_TRUE
        : HEMP_FALSE;
}

