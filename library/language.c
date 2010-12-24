#include <hemp/language.h>

hemp_language
hemp_language_new(
    hemp_hemp   hemp,
    hemp_string name,
    hemp_num    version
) {
    hemp_language language;
    HEMP_ALLOCATE(language);

    language->hemp    = hemp;
    language->name    = hemp_string_clone(name, "language name");
    language->version = version;
    
    return language;
}


void
hemp_language_free(
    hemp_language language
) {
    hemp_mem_free(language->name);
    hemp_mem_free(language);
}


HEMP_AUTOLOAD(hemp_language_autoload) {
    return hemp_use_module(factory->hemp, "language", name)
        ? HEMP_TRUE
        : HEMP_FALSE;
}

HEMP_AUTOLOAD(hemp_feature_autoload) {
    return hemp_use_module(factory->hemp, "feature", name)
        ? HEMP_TRUE
        : HEMP_FALSE;
}