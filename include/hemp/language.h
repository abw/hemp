#ifndef HEMP_LANGUAGE_H
#define HEMP_LANGUAGE_H

#include <hemp/core.h>
#include <hemp/factory.h>
#include <hemp/module.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_language {
    Hemp            hemp;
    HempString      name;
    HempNum         version;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempLanguage 
hemp_language_new(
    Hemp            hemp,
    HempString      name,
    HempNum         version
);

void 
hemp_language_free(
    HempLanguage    language
);

#endif /* HEMP_LANGUAGE_H */
