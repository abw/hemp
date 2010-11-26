#ifndef HEMP_LANGUAGES_H
#define HEMP_LANGUAGES_H

#include <hemp/core.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_language {
    hemp_hemp           hemp;
    hemp_string         name;
    hemp_num            version;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_language 
    hemp_language_new(
        hemp_hemp       hemp,
        hemp_string     name,
        hemp_num        version
);

void 
    hemp_language_free(
        hemp_language   language
    );


/*--------------------------------------------------------------------------
 * core hemp language
 *--------------------------------------------------------------------------*/

hemp_language hemp_language_hemp_init(hemp_hemp hemp, hemp_string name);
hemp_grammar  hemp_grammar_hemp_alpha(hemp_hemp, hemp_string);
hemp_grammar  hemp_grammar_hemp_bravo(hemp_hemp, hemp_string);
hemp_grammar  hemp_grammar_hemp_charlie(hemp_hemp, hemp_string);


/*--------------------------------------------------------------------------
 * tt3 language
 *--------------------------------------------------------------------------*/

hemp_language hemp_language_tt3_init(hemp_hemp hemp, hemp_string name);


#endif /* HEMP_LANGUAGES_H */
