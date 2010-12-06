#include <hemp/scope.h>


hemp_scope
hemp_scope_new(
    hemp_hemp hemp
) {
    hemp_scope scope;
    HEMP_ALLOCATE(scope);

    scope->hemp    = hemp;
    scope->parent  = NULL;
    scope->context = hemp_context_new(hemp);

    return scope;
}


void
hemp_scope_free(
    hemp_scope scope
) {
    // TODO: worry about cleanup
    hemp_context_free(scope->context);
    hemp_mem_free(scope);
}

