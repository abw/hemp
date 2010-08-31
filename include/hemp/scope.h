#ifndef HEMP_SCOPE_H
#define HEMP_SCOPE_H

#include <hemp/core.h>
#include <hemp/context.h>

struct hemp_scope_s {
    hemp_p          hemp;
    hemp_scope_p    parent;
    hemp_context_p  context;
}; 


hemp_scope_p
    hemp_scope_init(
        hemp_p hemp
    );

void
    hemp_scope_free(
        hemp_scope_p scope
    );

#endif /* HEMP_SCOPE_H */
