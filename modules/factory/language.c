#include <hemp.h>
#include <hemp/debug.h>


HEMP_FACTORY(hemp_language_factory);
HEMP_HASH_ITERATOR(hemp_language_cleaner);


/*--------------------------------------------------------------------------
 * Binder function registers the language factory with the hemp instance
 *--------------------------------------------------------------------------*/

HempBool
hemp_module_binder(
    HempModule  module,
    Hemp        hemp
) {
    hemp_register_factory( hemp, HEMP_FACTORY_LANGUAGE, &hemp_language_factory );
    hemp_debug_init("loaded language factory module\n");
    return HEMP_TRUE;
}



/*--------------------------------------------------------------------------
 * Factory functions for loading languages
 *--------------------------------------------------------------------------*/

HEMP_FACTORY(hemp_language_factory) {
    hemp_debug_init("instantiating language factory\n");
    HempFactory factory = hemp_factory_new(hemp, name);
    factory->cleaner    = hemp_language_cleaner;
    return factory;
}


HEMP_HASH_ITERATOR(hemp_language_cleaner) {
    HempLanguage language = (HempLanguage) hemp_val_ptr(item->value);
    hemp_debug_init("cleaning language: %s\n", language->name);
    hemp_language_free(language);
    return HEMP_TRUE;
}


