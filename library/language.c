#include <hemp/language.h>

/*--------------------------------------------------------------------------
 * Factory functions for loading languages
 *--------------------------------------------------------------------------*/

HEMP_FACTORY(hemp_language_factory) {
    hemp_debug_init("instantiating language factory\n");
    HempFactory factory = hemp_factory_new(hemp, name);
    factory->cleaner     = hemp_language_cleaner;
    return factory;
}


HEMP_HASH_ITERATOR(hemp_language_cleaner) {
    HempLanguage language = (HempLanguage) hemp_val_ptr(item->value);
    hemp_debug_init("cleaning language: %s\n", language->name);
    hemp_language_free(language);
    return HEMP_TRUE;
}


/*--------------------------------------------------------------------------
 * 
 *--------------------------------------------------------------------------*/


HempLanguage
hemp_language_new(
    Hemp   hemp,
    HempString name,
    HempNum    version
) {
    HempLanguage language;
    HEMP_ALLOCATE(language);

    language->hemp    = hemp;
    language->name    = hemp_string_clone(name, "language name");
    language->version = version;
    
    return language;
}


void
hemp_language_free(
    HempLanguage language
) {
    hemp_mem_free(language->name);
    hemp_mem_free(language);
}




/*--------------------------------------------------------------------------
 * Language features
 *--------------------------------------------------------------------------*/

HEMP_FACTORY(hemp_feature_factory) {
    hemp_debug_init("instantiating feature factory\n");
    HempFactory factory = hemp_factory_new(hemp, name);
    return factory;
}

