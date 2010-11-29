#ifndef HEMP_NAMESPACE_H
#define HEMP_NAMESPACE_H

#include <hemp/core.h>
#include <hemp/type/hash.h>


struct hemp_namespace {
    hemp_u16        id;
    hemp_string     name;
    hemp_namespace  parent;
    hemp_hash       children;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

void 
hemp_global_namespace_init(
    hemp_global     global
);

void 
hemp_global_namespace_free(
    hemp_global     global
);

hemp_namespace
hemp_namespace_init(
    hemp_u16        id,
    hemp_string     name
);

hemp_namespace
hemp_namespace_subspace(
    hemp_hash       hash,
    hemp_string     name,
    hemp_namespace  parent
);

hemp_namespace
hemp_resolve_namespace(
    hemp_string     fullname
);

void
hemp_namespace_free(
    hemp_namespace  namespace
);

hemp_bool
hemp_namespace_free_child(
    hemp_hash       namespaces,
    hemp_pos        position,
    hemp_slot       item
);



/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_namespace_next_id()                                \
    (++HempGlobal.namespace_id)


#define hemp_namespace_root(name)                               \
    hemp_namespace_subspace(                                    \
        HempGlobal.namespaces, name, NULL                       \
    )

#define hemp_namespace_instance(name) (                         \
    strchr(name, HEMP_DOT)                                      \
        ? hemp_resolve_namespace(name)                          \
        : hemp_namespace_root(name)                             \
)

#define hemp_namespace_id(name) (                               \
    hemp_namespace_instance(name)->id                           \
)

#define hemp_namespace_child(namespace, name)                   \
    hemp_namespace_subspace(                                    \
        namespace->children, name, namespace                    \
    )


#endif /* HEMP_NAMESPACE_H */
