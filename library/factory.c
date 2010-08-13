#include <hemp/factory.h>

hemp_char_t _hemp_factory_name_buffer[HEMP_BUFFER_SIZE];


hemp_factory_p
hemp_factory_init() {
    hemp_factory_p factory = (hemp_factory_p) hemp_mem_alloc(
        sizeof(struct hemp_factory_s)
    );

    if (! factory)
        hemp_mem_fail("factory");

    factory->instances    = hemp_hash_init();
    factory->constructors = hemp_hash_init();
    factory->cleaner      = NULL;
    
    return factory;
}


void
hemp_factory_free(
    hemp_factory_p factory
) {
    /* run any custom instance cleaner */
    if (factory->cleaner)
        hemp_hash_each(factory->instances, factory->cleaner);

    /* put our own house in order */
    hemp_hash_free(factory->instances);
    hemp_hash_each(factory->constructors, &hemp_factory_free_constructor);
    hemp_hash_free(factory->constructors);
    hemp_mem_free(factory);
}


hemp_action_p
hemp_factory_register(
    hemp_factory_p  factory,
    hemp_cstr_p     name,
    hemp_actor_f    actor,
    hemp_mem_p      script
) {
    hemp_action_p action = hemp_hash_fetch(
        factory->constructors, name
    );

    if (action)
        hemp_action_free(action);

    action = hemp_action_init(
        actor, script
    );

//  hemp_hash_store("registering %s action at %p in %p\n", name, action, factory->constructors);

    hemp_hash_store(
        factory->constructors, name, action
    );
    
    return action;
}


hemp_action_p
hemp_factory_constructor(
    hemp_factory_p  factory,
    hemp_cstr_p     name
) {
    hemp_debug_call("hemp_factory_constructor(F, %s)\n", name);
    
    static hemp_char_t  wildname[HEMP_BUFFER_SIZE];
    hemp_list_p splits;
    
    hemp_action_p constructor = hemp_hash_fetch(
        factory->constructors, name
    );

    if (constructor)
        return constructor;

    /* if we didn't find a constructor for what we wanted, e.g. foo.bar.baz
     * then we look for a wildcard entry, longest first and give it a chance
     * to construct the constructor, e.g. ask foo.bar.* to construct baz, and 
     * if it declines, see if there's a foo.* to construct bar.baz
     */
    if ((splits = hemp_cstr_splits(name, HEMP_STR_DOT))) {
        int n;
        hemp_cstr_split_p split;
        hemp_action_p wildcard;
            
        for (n = splits->length - 1; n >= 0; n--) {
            split = (hemp_cstr_split_p) hemp_list_item(splits, n);
            snprintf(wildname, HEMP_BUFFER_SIZE, "%s.*", split->left);

            /* look for a wildcard meta-constructor */
            wildcard = hemp_hash_fetch(
                factory->constructors, wildname
            );
            if (! wildcard)
                continue;               /* no dice, try again               */

            /* see if it can generate a constructor */
            constructor = hemp_action_run(
//              wildcard, split->right
                wildcard, name
            );
            if (constructor) {
                hemp_hash_store(
                    factory->constructors, name, constructor
                );
                break;
            }
        }
        hemp_list_free(splits);
    }

    return constructor;
}


hemp_mem_p
hemp_factory_instance(
    hemp_factory_p  factory,
    hemp_cstr_p     name
) {
    hemp_debug_call("hemp_factory_instance(F, %s)\n", name);
    
    static hemp_char_t  wildname[HEMP_BUFFER_SIZE];
    hemp_list_p splits;
    
    hemp_mem_p instance = hemp_hash_fetch(
        factory->instances, name
    );

    if (instance)
        return instance;

    hemp_action_p constructor = hemp_factory_constructor(
        factory, name
    );

    if (constructor) {
        instance = hemp_action_run(
            constructor, name
        );

        /* TODO: not sure about name.  We used to use dialect->name which is 
        * guaranteed to be locally duplicated in a dialect */
        if (instance)
            hemp_hash_store(factory->instances, name, instance);
    }

    return instance;
}


hemp_bool_t
hemp_factory_free_constructor(
    hemp_hash_p         dialects,
    hemp_pos_t          position,
    hemp_hash_item_p    item
) {
    hemp_action_free( (hemp_action_p) item->value );
    return HEMP_TRUE;
}


