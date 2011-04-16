#include <hemp.h>
#include <hemp/debug.h>


HEMP_FACTORY(hemp_tag_factory);
HEMP_HASH_ITERATOR(hemp_tag_cleaner);


/*--------------------------------------------------------------------------
 * Binder function registers the tag factory with the hemp instance
 *--------------------------------------------------------------------------*/

HempBool
hemp_module_binder(
    HempModule  module,
    Hemp        hemp
) {
    hemp_register_factory( hemp, HEMP_FACTORY_TAG, &hemp_tag_factory );
    hemp_debug_msg("loaded tag factory module\n");
    return HEMP_TRUE;
}


/*--------------------------------------------------------------------------
 * Factory functions for loading tags
 *--------------------------------------------------------------------------*/

HEMP_FACTORY(hemp_tag_factory) {
    hemp_debug_init("instantiating tag factory: %s\n", name);
    HempFactory factory = hemp_factory_new(hemp, name);
    factory->cleaner    = hemp_tag_cleaner;
    return factory;
}


HEMP_HASH_ITERATOR(hemp_tag_cleaner) {
    HempTag tag = (HempTag) hemp_val_ptr(item->value);
    hemp_debug_init("cleaning tag: %s\n", tag->name);
    hemp_tag_free(tag);
    return HEMP_TRUE;
}



