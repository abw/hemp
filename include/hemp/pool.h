#ifndef HEMP_POOL_H
#define HEMP_POOL_H

#include "hemp/slab.h"


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_pool {
    hemp_slab_t slab;           /* pointer to slab(s) of memory         */
    hemp_ptr_t  next;           /* next available memory record         */
    hemp_size_t size;           /* size of each memory record           */
    hemp_size_t used;           /* number of records in use             */
    hemp_size_t capacity;       /* allocated storage capacity           */
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_pool_t 
    hemp_pool_init(
        hemp_size_t size,
        hemp_size_t capacity
    );

hemp_ptr_t
    hemp_pool_take(
        hemp_pool_t pool
    );

void 
    hemp_pool_grow(
        hemp_pool_t pool
    );

void 
    hemp_pool_free(
        hemp_pool_t pool
    );


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_pool_null(p)   \
    hemp_pool_free(p);      \
    p = NULL;                


#endif /* HEMP_POOL_H */

