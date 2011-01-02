#ifndef HEMP_LIBRARY_H
#define HEMP_LIBRARY_H

#include "hemp/core.h"


/* NOT IN USE */

/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_library {
    HempString     name;
    
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_library
hemp_library_new(
    HempString     name
);



#endif /* HEMP_LIBRARY_H */
