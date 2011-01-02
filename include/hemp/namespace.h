#ifndef HEMP_NAMESPACE_H
#define HEMP_NAMESPACE_H

#include <hemp/core.h>
#include <hemp/type/hash.h>


struct hemp_namespace {
    HempU16        id;
    HempString     path;
    HempString     name;
    HempNamespace  parent;
    HempHash       children;
};

//struct hemp_namespaces {
//    HempU16        next_id;
//    HempString     name;
//    HempHash       namespaces;
//};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempNamespace
hemp_namespace_init(
    HempU16        id,
    HempString     name,
    HempNamespace  parent
);

HempNamespace
hemp_namespace_child(
    HempNamespace  namespace,
    HempString     name
);

HempNamespace
hemp_resolve_namespace(
    HempString     path
);

void
hemp_namespace_free(
    HempNamespace  namespace
);

HempBool
hemp_namespace_free_child(
    HempHash       namespaces,
    HempPos        position,
    HempSlot       item
);




/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_namespace_next_id()                                \
    (++HempGlobalData.namespace_id)


#define hemp_namespace_root(name)                               \
    hemp_namespace_child(                                       \
        HempGlobalData.namespace, name                          \
    )

#define hemp_namespace_instance(name) (                         \
    strchr(name, HEMP_DOT)                                      \
        ? hemp_resolve_namespace(name)                          \
        : hemp_namespace_root(name)                             \
)

#define hemp_namespace_id(name) (                               \
    hemp_namespace_instance(name)->id                           \
)


#endif /* HEMP_NAMESPACE_H */
