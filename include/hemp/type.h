#ifndef HEMP_TYPE_H
#define HEMP_TYPE_H

#include <string.h>
#include <stdlib.h>
#include "hemp/memory.h"


/*--------------------------------------------------------------------------
 * Define the base members that any type record must have.  
 *   name   unique, human-readable name, e.g. HempText, HempList, etc
 *   base   pointer to the base type, NULL for the uber type (or self-ref?)
 *   size   size in bytes of an instance of this type
 *   init   initialisation (but not allocation) function to prepare instance
 *   wipe   cleanup (but not de-allocation) function to release instance
 *--------------------------------------------------------------------------*/

#define HEMP_TYPE_BASE      \
    hemp_name_t  name;      \
    hemp_type_t  base;      \
    hemp_size_t  size;      \
    hemp_init_fn init;      \
    hemp_wipe_fn wipe;

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


struct hemp_type {
    HEMP_TYPE_BASE
};

extern hemp_type_t HempType;


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/


hemp_ptr_t  hemp_new(hemp_type_t type, ...);
void        hemp_old(hemp_ptr_t item);



/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define HEMP_TYPE_HEAD  sizeof(hemp_type_t)
#define HEMP_ARGS       va_list *_hemp_args
#define HEMP_ARG(type)  va_arg(*_hemp_args, type)


/* Returns the the address in memory of the type pointer positioned 
 * immediately before the object pointer passed as an argument.  The 
 * object must be created via hemp_new() to ensure this arrangement.
 */
#define hemp_type_at(obj_ptr) (             \
    ((char *) obj_ptr - HEMP_TYPE_HEAD)     \
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

