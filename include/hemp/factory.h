#ifndef HEMP_FACTORY_H
#define HEMP_FACTORY_H

#include <hemp/action.h>
#include <hemp/module.h>
#include <hemp/type/hash.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_factory {
    Hemp            hemp;
    HempString      name;
    HempHash        instances;
    HempHash        constructors;
    hemp_hash_iter  cleaner;
    HempAutoload    autoload;
}; 


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HEMP_FACTORY(hemp_meta_factory);
HEMP_AUTOLOAD(hemp_factory_autoload);

HempFactory 
hemp_factory_new(
    Hemp            hemp,
    HempString      name
);

void
hemp_factory_free(
    HempFactory     factory
);

HempBool
hemp_factory_free_constructor(
    HempHash        constructors,
    HempPos         position,
    HempSlot        slot
);

HempAction
hemp_factory_register(
    HempFactory     factory,
    HempString      name,
    HempActor       actor,
    HempMemory      script
);

HempAction
hemp_factory_constructor(
    HempFactory     factory,
    HempString      name
);

HempMemory
hemp_factory_instance(
    HempFactory     factory,
    HempString      name
);

HempBool
hemp_meta_factory_cleaner(
    HempHash        factories,
    HempPos         position,
    HempSlot        item
);


#endif /* HEMP_FACTORY_H */
