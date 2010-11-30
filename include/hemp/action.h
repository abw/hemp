#ifndef HEMP_ACTION_H
#define HEMP_ACTION_H

#include <hemp/core.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_action {
    hemp_actor      actor;
    hemp_memory     script;
}; 


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_action
hemp_action_init(
    hemp_action     action,
    hemp_actor      actor,
    hemp_memory     script
);

void
hemp_action_free(
    hemp_action     action
);


/*--------------------------------------------------------------------------
 * macro definitions
 *--------------------------------------------------------------------------*/

#define hemp_action_new(actor, script) \
    hemp_action_init(NULL, actor, script)

#define hemp_action_run(action, ...) \
    action->actor(action->script, __VA_ARGS__)


#endif /* HEMP_ACTION_H */
