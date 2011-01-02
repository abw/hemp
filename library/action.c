#include <hemp/action.h>


HempAction
hemp_action_init(
    HempAction      action,
    HempActor       actor,
    HempMemory      script
) {
    HEMP_ALLOCATE(action);
    action->actor  = actor;
    action->script = script;
    return action;
}


void
hemp_action_free(
    HempAction action
) {
    /* TODO: add a cleaner for any dynamically allocated memory */
    hemp_mem_free(action);
}
