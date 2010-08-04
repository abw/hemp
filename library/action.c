#include <hemp/action.h>


hemp_action_p
hemp_action_init(
    hemp_actor_f actor,
    hemp_mem_p   script
) {
    hemp_action_p action = (hemp_action_p) hemp_mem_alloc(
        sizeof(struct hemp_action_s)
    );

    if (! action)
        hemp_mem_fail("action");

    action->actor  = actor;
    action->script = script;
    
    return action;
}


void
hemp_action_free(
    hemp_action_p action
) {
    // TODO: do we need to add a cleaner in case script (or actor?) has been 
    // dynamically allocated?
    hemp_mem_free(action);
}
