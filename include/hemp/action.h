#ifndef HEMP_ACTION_H
#define HEMP_ACTION_H

#include <hemp/core.h>



/*--------------------------------------------------------------------------
 * data structures and type definitions
 *--------------------------------------------------------------------------*/

struct hemp_action_s {
    hemp_actor_f actor;
    hemp_mem_p   script;
}; 



/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_action_p 
    hemp_action_init(
        hemp_actor_f actor,
        hemp_mem_p   script
    );

void
    hemp_action_free(
        hemp_action_p action
    );


/*--------------------------------------------------------------------------
 * macro definitions
 *--------------------------------------------------------------------------*/

#define hemp_action_run(action, ...) \
    action->actor(action->script, __VA_ARGS__)


#endif /* HEMP_ACTION_H */
