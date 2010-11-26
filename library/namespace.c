#include <hemp/namespace.h>


void hemp_global_namespace_init(
    hemp_global global
) {
    /* return silently if we've already done this */
    if (global->namespaces)
        return;

    global->namespace_id = 0;
    global->namespaces   = hemp_hash_init();
}


void hemp_global_namespace_free(
    hemp_global global
) {
    /* return silently if this has already been done */
    if (! global->namespaces)
        return;

    hemp_hash_each(global->namespaces, &hemp_namespace_free_child);
    hemp_hash_free(global->namespaces);
    global->namespaces   = NULL;
    global->namespace_id = 0;
}

    
hemp_namespace_p
hemp_namespace_init(
    hemp_u16  id,
    hemp_string  name
) {
    hemp_namespace_p namespace = (hemp_namespace_p) hemp_mem_alloc(
        sizeof(struct hemp_namespace_s)
    );

    if (! namespace)
        hemp_mem_fail("namespace");

    namespace->id       = id;
    namespace->name     = hemp_string_clone(name, "namespace name");
    namespace->children = hemp_hash_init();
    namespace->parent   = NULL;

    return namespace;
}


hemp_namespace_p
hemp_namespace_instance(
    hemp_hash      hash,
    hemp_string       name,
    hemp_namespace_p parent
) {
    hemp_namespace_p    child;
    hemp_value        value;
    hemp_u16          id;

    value = hemp_hash_fetch(hash, name);

    if (hemp_is_missing(value)) {
        child = hemp_namespace_init(
            hemp_namespace_next_id(),
            name
        );
        child->parent = parent;
        hemp_hash_store_pointer(
            hash,
            child->name,    /* use the name the child allocated */
            child
        );
    }
    else {
        child = (hemp_namespace_p) hemp_val_ptr(value);
    }

    return child;
}


hemp_namespace_p
hemp_resolve_namespace(
    hemp_string       fullname
) {
    hemp_list      names = hemp_string_split(fullname, HEMP_STR_DOT);
    hemp_string       name  = hemp_val_str( hemp_list_item(names, 0) );
    hemp_namespace_p space = hemp_namespace_root(name);
    hemp_size      n;
    
    for (n = 1; n < names->length; n++) {
        name  = hemp_val_str( hemp_list_item(names, n) );
        space = hemp_namespace_child(space, name);
    }

    hemp_list_free(names);

    return space;
}


void
hemp_namespace_free(
    hemp_namespace_p namespace
) {
    hemp_mem_free(namespace->name);
    hemp_hash_each(namespace->children, &hemp_namespace_free_child);
    hemp_hash_free(namespace->children);
    hemp_mem_free(namespace);
}


hemp_bool
hemp_namespace_free_child(
    hemp_hash     namespaces,
    hemp_pos      position,
    hemp_slot     item
) {
    hemp_namespace_free( (hemp_namespace_p) hemp_val_ptr(item->value) );
    return HEMP_TRUE;
}
