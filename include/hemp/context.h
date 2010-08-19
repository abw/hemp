#ifndef HEMP_CONTEXT_H
#define HEMP_CONTEXT_H

#include <hemp/core.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_context_s {
    hemp_p  hemp;
/*  hemp_value_t    variables; */
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_context_p  hemp_context_init(hemp_p);
void            hemp_context_free(hemp_context_p);


#endif /* HEMP_CONTEXT_H */
