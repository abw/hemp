#include <hemp/action.h>


hemp_action
hemp_action_init(
    hemp_action     action,
    hemp_actor      actor,
    hemp_memory     script
) {
    HEMP_ALLOCATE(action);
    action->actor  = actor;
    action->script = script;
    return action;
}


void
hemp_action_free(
    hemp_action action
) {
    /* TODO: add a cleaner for any dynamically allocated memory */
    hemp_mem_free(action);
}
