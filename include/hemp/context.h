#ifndef HEMP_CONTEXT_H
#define HEMP_CONTEXT_H

#include <hemp/core.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_context_s {
/*    hemp_value_t    variables; */
    // more TODO
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_context_p  hemp_context_init();
void            hemp_context_free(hemp_context_p);


#endif /* HEMP_CONTEXT_H */
