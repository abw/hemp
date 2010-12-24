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

HEMP_AUTOLOAD(hemp_language_autoload);
HEMP_AUTOLOAD(hemp_feature_autoload);


#endif /* HEMP_LANGUAGE_H */
