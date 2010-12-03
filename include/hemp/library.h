#ifndef HEMP_LIBRARY_H
#define HEMP_LIBRARY_H

#include "hemp/core.h"


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_library {
    hemp_string     name;
    
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_library
hemp_library_new(
    hemp_string     name
);


void
hemp_module_free(
    hemp_module     module
);


hemp_bool
hemp_module_load(
    hemp_hemp       hemp,
    hemp_string     name
);


#endif /* HEMP_MODULE_H */
