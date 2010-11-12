#ifndef HEMP_NAMESPACE_H
#define HEMP_NAMESPACE_H

#include <hemp/core.h>
#include <hemp/type/hash.h>


struct hemp_namespace_s {
    hemp_u16_t          id;
    hemp_str_p          name;
    hemp_namespace_p    parent;
    hemp_hash_p         children;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

void 
    hemp_global_namespace_init(
        hemp_global_p   global
    );

void 
    hemp_global_namespace_free(
        hemp_global_p   global
    );

hemp_namespace_p
    hemp_namespace_init(
        hemp_u16_t      id,
        hemp_str_p      name
    );

hemp_namespace_p
    hemp_namespace_instance(
        hemp_hash_p      hash,
        hemp_str_p       name,
        hemp_namespace_p parent
    );

hemp_namespace_p
    hemp_resolve_namespace(
        hemp_str_p       fullname
    );

void
    hemp_namespace_free(
        hemp_namespace_p namespace
    );

hemp_bool_t
    hemp_namespace_free_child(
        hemp_hash_p     namespaces,
        hemp_pos_t      position,
        hemp_slot_p     item
    );



/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_namespace_next_id()                                \
    (++hemp_global.namespace_id)


#define hemp_namespace_root(name)                               \
    hemp_namespace_instance(                                    \
        hemp_global.namespaces, name, NULL                      \
    )

#define hemp_namespace(name) (                                  \
    strchr(name, HEMP_DOT)                                      \
        ? hemp_resolve_namespace(name)                          \
        : hemp_namespace_root(name)                             \
)

#define hemp_namespace_id(name) (                               \
    hemp_namespace(name)->id                                    \
)

#define hemp_namespace_child(namespace, name)                   \
    hemp_namespace_instance(                                    \
        namespace->children, name, namespace                    \
    )


#endif /* HEMP_NAMESPACE_H */
