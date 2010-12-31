#ifndef HEMP_FACTORY_H
#define HEMP_FACTORY_H

#include <hemp/action.h>
#include <hemp/module.h>
#include <hemp/type/hash.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

typedef hemp_bool 
(* hemp_autoload)(
    hemp_factory    factory,
    hemp_string     name
);

struct hemp_factory {
    hemp_hemp       hemp;
    hemp_string     name;
    hemp_hash       instances;
    hemp_hash       constructors;
    hemp_hash_iter  cleaner;
    hemp_autoload   autoload;
}; 


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HEMP_FACTORY(hemp_meta_factory);
HEMP_AUTOLOAD(hemp_factory_autoload);

hemp_factory 
hemp_factory_new(
    hemp_hemp       hemp,
    hemp_string     name
);

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

hemp_bool
hemp_meta_factory_cleaner(
    hemp_hash       factories,
    hemp_pos        position,
    hemp_slot       item
);




#endif /* HEMP_FACTORY_H */
