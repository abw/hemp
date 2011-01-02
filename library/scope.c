#include <hemp/scope.h>


HempScope
hemp_scope_new(
    Hemp hemp
) {
    HempScope scope;
    HEMP_ALLOCATE(scope);

    scope->hemp    = hemp;
    scope->parent  = NULL;
    scope->context = hemp_context_new(hemp);

    return scope;
}


void
hemp_scope_free(
    HempScope scope
) {
    // TODO: worry about cleanup
    hemp_context_free(scope->context);
    hemp_mem_free(scope);
}

