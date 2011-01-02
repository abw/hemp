#ifndef HEMP_SCOPE_H
#define HEMP_SCOPE_H

#include <hemp/core.h>
#include <hemp/context.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_scope {
    Hemp       hemp;
    HempScope      parent;
    HempContext    context;
}; 


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempScope
    hemp_scope_new(
        Hemp   hemp
    );

void
    hemp_scope_free(
        HempScope  scope
    );


#endif /* HEMP_SCOPE_H */
