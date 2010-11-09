#ifndef HEMP_NAMESPACE_H
#define HEMP_NAMESPACE_H

#include <hemp/core.h>
#include <hemp/type/hash.h>


struct hemp_namespace_s {
    hemp_p              hemp;
    hemp_u16_t          id;
    hemp_str_p          name;
    hemp_namespace_p    parent;
    hemp_hash_p         children;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_namespace_p
    hemp_namespace_init(
        hemp_p      hemp,
        hemp_u16_t  id,
        hemp_str_p  name
    );

hemp_bool_t
    hemp_namespace_free_child(
        hemp_hash_p     namespaces,
        hemp_pos_t      position,
        hemp_slot_p     item
    );

void
    hemp_namespace_free(
        hemp_namespace_p namespace
    );

hemp_namespace_p
    hemp_namespace_instance(
        hemp_p           hemp,
        hemp_hash_p      hash,
        hemp_str_p       name,
        hemp_namespace_p parent
    );


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_namespace_next_id(hemp)                            \
    (++hemp->namespace_id)


#define hemp_namespace(hemp, name)                              \
    hemp_namespace_instance(                                    \
        hemp, hemp->namespaces, name, NULL                      \
    )

#define hemp_namespace_child(namespace, name)                   \
    hemp_namespace_instance(                                    \
        namespace->hemp, namespace->children, name, namespace   \
    )


#endif /* HEMP_NAMESPACE_H */
