#include <hemp/language.h>

hemp_language_p
hemp_language_init(
    hemp_p      hemp,
    hemp_cstr_p name,
    hemp_num_t  version
) {
    hemp_language_p language = (hemp_language_p) hemp_mem_alloc(
        sizeof(struct hemp_language_s)
    );

    if (! language)
        hemp_mem_fail("language");

    language->hemp    = hemp;
    language->name    = hemp_cstr_clone(name, "language name");
    language->version = version;
    
    return language;
}


void
hemp_language_free(
    hemp_language_p language
) {
    hemp_mem_free(language->name);
    hemp_mem_free(language);
}
