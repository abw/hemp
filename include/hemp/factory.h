#ifndef HEMP_FACTORY_H
#define HEMP_FACTORY_H

#include <hemp/action.h>
#include <hemp/type/hash.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_factory {
    hemp_hash           instances;
    hemp_hash           constructors;
    hemp_hash_iter      cleaner;
}; 


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_factory 
    hemp_factory_new();

void
    hemp_factory_free(
        hemp_factory    factory
    );

hemp_bool
    hemp_factory_free_constructor(
        hemp_hash       constructors,
        hemp_pos        position,
        hemp_slot       slot
    );

hemp_action
    hemp_factory_register(
        hemp_factory    factory,
        hemp_string     name,
        hemp_actor      actor,
        hemp_memory     script
    );

hemp_action
    hemp_factory_constructor(
        hemp_factory    factory,
        hemp_string     name
    );

hemp_memory
    hemp_factory_instance(
        hemp_factory    factory,
        hemp_string     name
    );


#endif /* HEMP_FACTORY_H */
