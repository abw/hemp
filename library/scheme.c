#include <hemp/scheme.h>


HEMP_FACTORY(hemp_scheme_factory) {
    hemp_debug_init("instantiating scheme factory: %s\n", name);
    hemp_factory factory = hemp_factory_new(hemp, name);
    factory->cleaner     = hemp_scheme_cleaner;
    return factory;
}


HEMP_HASH_ITERATOR(hemp_scheme_cleaner) {
    hemp_scheme scheme = (hemp_scheme) hemp_val_ptr(item->value);
    hemp_debug_init("cleaning scheme: %s\n", scheme->name);
    hemp_scheme_free(scheme);
    return HEMP_TRUE;
}


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


