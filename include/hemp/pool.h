#ifndef HEMP_POOL_H
#define HEMP_POOL_H

#include <hemp/slab.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_pool_s {
    hemp_slab_p slab;           /* pointer to slab(s) of memory         */
    hemp_mem_p  next;           /* next available memory record         */
    hemp_size_t size;           /* size of each memory record           */
    hemp_size_t used;           /* number of records in use             */
    hemp_size_t capacity;       /* allocated storage capacity           */
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_pool_p
    hemp_pool_init(
        hemp_size_t size,
        hemp_size_t capacity
    );

hemp_mem_p
    hemp_pool_take(
        hemp_pool_p pool
    );

void 
    hemp_pool_grow(
        hemp_pool_p pool
    );

void 
    hemp_pool_free(
        hemp_pool_p pool
    );

void
    hemp_pool_each(
        hemp_pool_p      pool,
        hemp_pool_each_f func
    );



#endif /* HEMP_POOL_H */

