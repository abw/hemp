#ifndef HEMP_TYPE_H
#define HEMP_TYPE_H

#include <string.h>
#include <stdlib.h>
#include "hemp/memory.h"


/*--------------------------------------------------------------------------
 * All types must define the following members:
 *   name       A unique, human-readable name, e.g. HempText, HempList, etc
 *   size       The size in bytes of an instance of this type
 *   parent     A pointer to the parent (base) type, or NULL (or a self-ref?)
 *              in the case of the HempType uber type.
 *   acquire    A function to allocate memory for an instance of this type,
 *              either directly to malloc(), from a slab/pool or otherwise
 *   prepare    An initialise function to prepare a freshly allocated (or 
 *              re-used) object for use.  May allocate extra memory.
 *   cleanup    The reverse of prepare, for de-allocating any extra memory
 *              created within the object and performing any other cleanup.
 *   release    A function to release the memory for the object itself, either
 *              directly to free() or back into a free pool.
 *--------------------------------------------------------------------------*/

#define HEMP_TYPE_BASE          \
    hemp_name_t     name;       \
    hemp_size_t     size;       \
    hemp_type_t     parent;     \
    hemp_acquire_fn acquire;    \
    hemp_prepare_fn prepare;    \
    hemp_cleanup_fn cleanup;    \
    hemp_release_fn release;

struct hemp_type {
    HEMP_TYPE_BASE
};

extern hemp_type_t HempType;


/* we may also want to define a custom head size so that we allocate some
 * additional memory before the pointer (in addition to the HEMP_TYPE_HEAD
 * bytes added for the type pointer) for types to use (e.g. capacity, length
 * for strings, lists, etc)
 */

/*  # other things we may want to add/extend into types 
    hemp_pool_t  pool;  
    hemp_value_fn value;
    compare, print, getter, setter,
    as number/sequence/mapping, attributes.
    dead, live, used, etc
    roles
*/



/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_type_t hemp_type_init(
    hemp_name_t name,
    hemp_size_t size
);

hemp_type_t hemp_type_child(
    hemp_type_t type,
    hemp_name_t name,
    hemp_size_t size
);

hemp_type_t hemp_type_free(
    hemp_type_t type
);

hemp_item_t hemp_new(
    hemp_type_t type, 
    ...
);

void hemp_old(
    hemp_ptr_t item
);

hemp_item_t hemp_item_acquire(
    hemp_type_t type
);

void hemp_item_release(
    hemp_item_t item
);



/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define HEMP_TYPE_SIZE  sizeof(hemp_type_t)
#define HEMP_ARGS       va_list *_hemp_args
#define HEMP_ARG(type)  va_arg(*_hemp_args, type)


/* Returns the the address in memory of the type pointer positioned 
 * immediately before the object pointer passed as an argument.  The 
 * object must be created via hemp_new() to ensure this arrangement.
 */
#define hemp_type_at(obj_ptr) (             \
    ((char *) obj_ptr - HEMP_TYPE_SIZE)     \
)

/* De-references the type pointer returned by hemp_type_at() to give a 
 * hemp_type_t pointer to hemp_type data structure for the object.
 */
#define hemp_type_of(obj_ptr) (             \
    (hemp_type_t) *(                        \
        (hemp_type_t *)                     \
        hemp_type_at(obj_ptr)               \
    )                                       \
)

/* Returns the name of the type returned by hemp_type_of()
 */
#define hemp_type_name(obj_ptr)             \
    hemp_type_of(obj_ptr)->name


#endif /* HEMP_TYPE_H */

