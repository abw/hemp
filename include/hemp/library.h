#ifndef HEMP_LIBRARY_H
#define HEMP_LIBRARY_H

#include "hemp/core.h"


/* NOT IN USE */

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



#endif /* HEMP_LIBRARY_H */
