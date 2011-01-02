#ifndef HEMP_ACTION_H
#define HEMP_ACTION_H

#include <hemp/core.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_action {
    HempActor       actor;
    HempMemory      script;
}; 


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempAction
hemp_action_init(
    HempAction      action,
    HempActor       actor,
    HempMemory      script
);

void
hemp_action_free(
    HempAction      action
);


/*--------------------------------------------------------------------------
 * macro definitions
 *--------------------------------------------------------------------------*/

#define hemp_action_new(actor, script)          \
    hemp_action_init(NULL, actor, script)

#define hemp_action_run(action, ...)            \
    action->actor(action->script, __VA_ARGS__)


#endif /* HEMP_ACTION_H */
