#include <hemp/scope.h>


hemp_scope_p
hemp_scope_init(
    hemp_p hemp
) {
    hemp_scope_p scope = (hemp_scope_p) hemp_mem_alloc(
        sizeof(struct hemp_scope_s)
    );
    
    if (! scope)
        hemp_mem_fail("scope");

    scope->hemp    = hemp;
    scope->parent  = NULL;
    scope->context = hemp_context_init(hemp);

    return scope;
}


void
hemp_scope_free(
    hemp_scope_p scope
) {
    // TODO: worry about cleanup
    hemp_context_free(scope->context);
    hemp_mem_free(scope);
}

