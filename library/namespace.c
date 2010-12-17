#include <hemp/namespace.h>


hemp_namespace
hemp_namespace_init(
    hemp_u16        id,
    hemp_string     name,
    hemp_namespace  parent
) {
    hemp_namespace  namespace;
    hemp_string     path;
    hemp_size       blen, plen, slen;
    HEMP_ALLOCATE(namespace);

    /* better to create a blank name for root N/S rather than leave it NULL */
    if (! name)
        name = HEMP_STR_BLANK;

    namespace->id       = id;
    namespace->children = hemp_hash_new();
    namespace->parent   = parent;

    if (parent && (blen = strlen(parent->path))) {
        slen = strlen(HEMP_STR_NAMESEP);
        plen = blen             /* parent (base) path length */
             + slen             /* separator length          */
             + strlen(name)     /* extra name component      */
             + 1;               /* terminating NUL           */
//      hemp_debug(
//          "[%s][%s][%s] = [%d][%d][%d] = [%d]\n", 
//          parent->path, HEMP_STR_NAMESEP, name,
//          blen, slen, strlen(name), plen
//      );
        path = hemp_mem_alloc(plen);

        if (! path)
            hemp_mem_fail("namespace name");

        strcpy(path, parent->path);
        strcat(path, HEMP_STR_NAMESEP);
        strcat(path, name);

        namespace->path = path;
        namespace->name = path + blen + slen;
    }
    else {
        namespace->path = hemp_string_clone(name, "namespace name");
        namespace->name = namespace->path;
    }

    return namespace;
}


hemp_namespace
hemp_namespace_child(
    hemp_namespace  parent,
    hemp_string     name
) {
    hemp_namespace  child;
    hemp_value      value;

    hemp_debug_call("namespace child: %p [%s] . [%s]\n", parent, parent->name, name);

    value = hemp_hash_fetch(parent->children, name);

    if (hemp_is_missing(value)) {
        child = hemp_namespace_init(
            hemp_namespace_next_id(),
            name,
            parent
        );
        child->parent = parent;
        hemp_hash_store_pointer(
            parent->children,
            child->name,    /* use the name the child allocated */
            child
        );
    }
    else {
        child = (hemp_namespace) hemp_val_ptr(value);
    }

    return child;
}


hemp_namespace
hemp_resolve_namespace(
    hemp_string path
) {
    hemp_debug_call("resolving namespace: %s\n", path);
    hemp_list      names = hemp_string_split(path, HEMP_STR_DOT);
    hemp_string    name  = hemp_val_str( hemp_list_item(names, 0) );
    hemp_namespace space = hemp_namespace_root(name);
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
    hemp_namespace namespace
) {
    hemp_mem_free(namespace->path);
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
    hemp_namespace_free( (hemp_namespace) hemp_val_ptr(item->value) );
    return HEMP_TRUE;
}
