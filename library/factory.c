#include <hemp/factory.h>

hemp_char _hemp_factory_name_buffer[HEMP_BUFFER_SIZE];


hemp_factory
hemp_factory_new(
    hemp_hemp   hemp
) {
    hemp_factory factory;
    HEMP_ALLOCATE(factory);
    factory->hemp         = hemp;
    factory->instances    = hemp_hash_new();
    factory->constructors = hemp_hash_new();
    factory->cleaner      = NULL;
    factory->autoload     = NULL;
    return factory;
}


void
hemp_factory_free(
    hemp_factory factory
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


hemp_action
hemp_factory_register(
    hemp_factory    factory,
    hemp_string     name,
    hemp_actor      actor,
    hemp_memory     script
) {
    hemp_action action = (hemp_action) hemp_hash_fetch_pointer(
        factory->constructors, name
    );

    if (action)
        hemp_action_free(action);

    action = hemp_action_new(
        actor, script
    );

//  hemp_hash_store("registering %s action at %p in %p\n", name, action, factory->constructors);

    hemp_hash_store_pointer(
        factory->constructors, name, action
    );
    
    return action;
}


hemp_action
hemp_factory_constructor(
    hemp_factory    factory,
    hemp_string     name
) {
    hemp_debug_factory("hemp_factory_constructor(%p, %s)\n", factory, name);
    
    static hemp_char    wildname[HEMP_BUFFER_SIZE];
    hemp_list           splits;
    hemp_autoload       autoload;
    
    hemp_action constructor = (hemp_action) hemp_hash_fetch_pointer(
        factory->constructors, name
    );

    if (constructor)
        return constructor;

    /* if we didn't find a constructor for what we wanted, e.g. foo.bar.baz
     * then we look for a wildcard entry, longest first and give it a chance
     * to construct the constructor, e.g. ask foo.bar.* to construct baz, and 
     * if it declines, see if there's a foo.* to construct bar.baz
     */

    splits   = hemp_string_splits(name, HEMP_STR_DOT);
    autoload = NULL;

lookup:
    if (splits) {
        int n;
        hemp_str_split split;
        hemp_action wildcard;
            
        for (n = splits->length - 1; n >= 0; n--) {
            split = (hemp_str_split) hemp_val_ptr( hemp_list_item(splits, n) );
            snprintf((char *) wildname, HEMP_BUFFER_SIZE, "%s.*", split->left);

            hemp_debug_factory("factory looking for [%s]\n", wildname);

            /* look for a wildcard meta-constructor */
            wildcard = (hemp_action) hemp_hash_fetch_pointer(
                factory->constructors, (hemp_string) wildname
            );
            if (! wildcard)
                continue;               /* no dice, try again               */


            hemp_debug_factory("factory calling constructor: %p\n", wildcard);

            /* see if it can generate a constructor */
            constructor = hemp_action_run(
//              wildcard, split->right
                wildcard, name
            );
            if (constructor) {
                hemp_hash_store_pointer(
                    factory->constructors, name, constructor
                );
                break;
            }
        }
    }
    
    if ( ! constructor && ! autoload && ( autoload = factory->autoload ) ) {
        hemp_debug_factory("factory calling autoload method\n");

        if (autoload(factory, name)) {
            hemp_debug_factory("factory autoload returned true\n");
            /* autoload worked so look to see if our item is now available */
            constructor = (hemp_action) hemp_hash_fetch_pointer(
                factory->constructors, name
            );
            /* fall back to a wildcard provider that may have been installed */
            if (! constructor)
                goto lookup;            /* sorry */
        }
        else {
            hemp_debug_factory("factory autoload returned false\n");
        }
    }

    if (splits)
        hemp_list_free(splits);

    return constructor;
}


hemp_memory
hemp_factory_instance(
    hemp_factory factory,
    hemp_string     name
) {
    hemp_debug_factory("hemp_factory_instance(%p, %s)\n", factory, name);
    
    hemp_memory instance = hemp_hash_fetch_pointer(
        factory->instances, name
    );

    if (instance)
        return instance;

    hemp_action constructor = hemp_factory_constructor(
        factory, name
    );

    if (constructor) {
        instance = hemp_action_run(
            constructor, name
        );

        /* TODO: not sure about name.  We used to use dialect->name which is 
        * guaranteed to be locally duplicated in a dialect */
        if (instance)
            hemp_hash_store_pointer(factory->instances, name, instance);
    }

    return instance;
}


hemp_bool
hemp_factory_free_constructor(
    hemp_hash     constructors,
    hemp_pos      position,
    hemp_slot     item
) {
    hemp_action_free( (hemp_action) hemp_val_ptr(item->value) );
    return HEMP_TRUE;
}


