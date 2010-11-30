#ifndef HEMP_SCOPE_H
#define HEMP_SCOPE_H

#include <hemp/core.h>
#include <hemp/context.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_scope {
    hemp_hemp       hemp;
    hemp_scope      parent;
    hemp_context    context;
}; 


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_scope
    hemp_scope_new(
        hemp_hemp   hemp
    );

void
    hemp_scope_free(
        hemp_scope  scope
    );


#endif /* HEMP_SCOPE_H */
