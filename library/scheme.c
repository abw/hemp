#include <hemp/scheme.h>


HempScheme
hemp_scheme_new(
    Hemp        hemp,
    HempString  name
) {
    HempScheme scheme;
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
    HempScheme scheme
) {
    if (scheme->name)
        hemp_mem_free(scheme->name);

    hemp_mem_free(scheme);
}


HempString 
hemp_scheme_namer(
    HempSource source
) {
    return source->scheme->name;
}


