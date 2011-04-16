#include <hemp.h>
#include <hemp/debug.h>


HEMP_FACTORY(hemp_dialect_factory);
HEMP_HASH_ITERATOR(hemp_dialect_cleaner);


/*--------------------------------------------------------------------------
 * Binder function registers the dialect factory with the hemp instance
 *--------------------------------------------------------------------------*/

HempBool
hemp_module_binder(
    HempModule  module,
    Hemp        hemp
) {
    hemp_register_factory( hemp, HEMP_FACTORY_DIALECT, &hemp_dialect_factory );
    hemp_debug_init("loaded dialect factory module\n");
    return HEMP_TRUE;
}



/*--------------------------------------------------------------------------
 * Factory functions for loading dialects
 *--------------------------------------------------------------------------*/

HEMP_FACTORY(hemp_dialect_factory) {
    hemp_debug_init("instantiating dialect factory\n");
    HempFactory factory = hemp_factory_new(hemp, name);
    factory->cleaner     = &hemp_dialect_cleaner;
    factory->autoload    = NULL;        // TODO: load language
    return factory;
}


HEMP_HASH_ITERATOR(hemp_dialect_cleaner) {
    HempDialect dialect = (HempDialect) hemp_val_ptr(item->value);
    hemp_debug_init("cleaning dialect: %s\n", dialect->name);
    hemp_dialect_free(dialect);
    return HEMP_TRUE;
}

