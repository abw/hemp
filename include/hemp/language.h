#ifndef HEMP_LANGUAGES_H
#define HEMP_LANGUAGES_H

#include <hemp/core.h>

struct hemp_language_s {
    hemp_p      hemp;
    hemp_cstr_p name;
    hemp_num_t  version;
};

hemp_language_p 
hemp_language_init(
    hemp_p      hemp,
    hemp_cstr_p name,
    hemp_num_t  version
);

void 
hemp_language_free(
    hemp_language_p language
);

/*--------------------------------------------------------------------------
 * core hemp language
 *--------------------------------------------------------------------------*/

hemp_language_p hemp_language_hemp_init(hemp_p hemp, hemp_cstr_p name);
hemp_language_p hemp_language_tt3_init(hemp_p hemp, hemp_cstr_p name);


#endif /* HEMP_LANGUAGES_H */
