#include <hemp/language.h>


/*--------------------------------------------------------------------------
 * Object methods
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
