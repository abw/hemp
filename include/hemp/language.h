#ifndef HEMP_LANGUAGE_H
#define HEMP_LANGUAGE_H

#include <hemp/core.h>
#include <hemp/factory.h>
#include <hemp/module.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_language {
    Hemp       hemp;
    HempString     name;
    HempNum        version;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HEMP_FACTORY(hemp_language_factory);
HEMP_HASH_ITERATOR(hemp_language_cleaner);

HempLanguage 
hemp_language_new(
    Hemp       hemp,
    HempString     name,
    HempNum        version
);

void 
hemp_language_free(
    HempLanguage   language
);

HEMP_FACTORY(hemp_feature_factory);

#endif /* HEMP_LANGUAGE_H */
