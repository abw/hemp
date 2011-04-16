#include <hemp.h>
#include <hemp/debug.h>


HEMP_FACTORY(hemp_scheme_factory);
HEMP_HASH_ITERATOR(hemp_scheme_cleaner);


/*--------------------------------------------------------------------------
 * Binder function registers the scheme factory with the hemp instance
 *--------------------------------------------------------------------------*/

HempBool
hemp_module_binder(
    HempModule  module,
    Hemp        hemp
) {
    hemp_register_factory( hemp, HEMP_FACTORY_SCHEME, &hemp_scheme_factory );
    hemp_debug_init("loaded scheme factory module\n");
    return HEMP_TRUE;
}



/*--------------------------------------------------------------------------
 * Factory functions for loading schemes
 *--------------------------------------------------------------------------*/

HEMP_FACTORY(hemp_scheme_factory) {
    hemp_debug_init("instantiating scheme factory: %s\n", name);
    HempFactory factory = hemp_factory_new(hemp, name);
    factory->cleaner     = hemp_scheme_cleaner;
    return factory;
}


HEMP_HASH_ITERATOR(hemp_scheme_cleaner) {
    HempScheme scheme = (HempScheme) hemp_val_ptr(item->value);
    hemp_debug_init("cleaning scheme: %s\n", scheme->name);
    hemp_scheme_free(scheme);
    return HEMP_TRUE;
}


