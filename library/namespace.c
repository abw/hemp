#include <hemp/namespace.h>

void hemp_global_namespace_init() {
    /* return silently if we've already done this */
    if (hemp_global.namespaces)
        return;

    hemp_global.namespace_id = 0;
    hemp_global.namespaces   = hemp_hash_init();
}


void hemp_global_namespace_free() {
    /* return silently if this has already been done */
    if (! hemp_global.namespaces)
        return;

    hemp_global.namespace_id = 0;
    hemp_hash_free(hemp_global.namespaces);
    hemp_global.namespaces = NULL;
}

    
hemp_namespace_p
hemp_namespace_init(
    hemp_p      hemp,
    hemp_u16_t  id,
    hemp_str_p  name
) {
    hemp_namespace_p namespace = (hemp_namespace_p) hemp_mem_alloc(
        sizeof(struct hemp_namespace_s)
    );

    if (! namespace)
        hemp_mem_fail("namespace");

    namespace->hemp     = hemp;
    namespace->id       = id;
    namespace->name     = hemp_string_clone(name, "namespace name");
    namespace->children = hemp_hash_init();
    namespace->parent   = NULL;

    return namespace;
}


hemp_namespace_p
hemp_namespace_instance(
    hemp_p           hemp,
    hemp_hash_p      hash,
    hemp_str_p       name,
    hemp_namespace_p parent
) {
    hemp_namespace_p    child;
    hemp_value_t        value;
    hemp_u16_t          id;

    value = hemp_hash_fetch(hash, name);

    if (hemp_is_missing(value)) {
        child = hemp_namespace_init(
            hemp,
            hemp_namespace_next_id(hemp),
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
    hemp_p           hemp,
    hemp_str_p       fullname
) {
    hemp_list_p      names = hemp_string_split(fullname, HEMP_STR_DOT);
    hemp_str_p       name  = hemp_val_str( hemp_list_item(names, 0) );
    hemp_namespace_p space = hemp_namespace(hemp, name);
    hemp_size_t      n;
    
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


hemp_bool_t
hemp_namespace_free_child(
    hemp_hash_p     namespaces,
    hemp_pos_t      position,
    hemp_slot_p     item
) {
    hemp_namespace_free( (hemp_namespace_p) hemp_val_ptr(item->value) );
    return HEMP_TRUE;
}
