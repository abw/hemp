#ifndef HEMP_LANGUAGE_H
#define HEMP_LANGUAGE_H

#include <hemp/core.h>
#include <hemp/factory.h>
#include <hemp/module.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_language {
    hemp_hemp       hemp;
    hemp_string     name;
    hemp_num        version;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HEMP_FACTORY(hemp_language_factory);
HEMP_HASH_ITERATOR(hemp_language_cleaner);

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

HEMP_FACTORY(hemp_feature_factory);

#endif /* HEMP_LANGUAGE_H */
