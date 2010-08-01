#ifndef HEMP_CONTEXT_H
#define HEMP_CONTEXT_H

#include "hemp/types.h"
#include "hemp/memory.h"


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_context {
    hemp_value_t    variables;
    // more TODO
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_context_t  hemp_context_init();
void            hemp_context_free(hemp_context_t);


#endif /* HEMP_CONTEXT_H */
