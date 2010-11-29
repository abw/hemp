#ifndef HEMP_POOL_H
#define HEMP_POOL_H

#include <hemp/slab.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_pool {
    hemp_slab       slab;           /* pointer to slab(s) of memory         */
    hemp_memory     next;           /* next available memory record         */
    hemp_size       size;           /* size of each memory record           */
    hemp_size       used;           /* number of records in use             */
    hemp_size       capacity;       /* allocated storage capacity           */
    hemp_pool_iter  cleaner;        /* memory cleanup function              */
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_pool
hemp_pool_init(
    hemp_pool       pool,
    hemp_size       size,
    hemp_size       capacity,
    hemp_pool_iter  cleaner
);

hemp_memory
hemp_pool_take(
    hemp_pool       pool
);

void 
hemp_pool_grow(
    hemp_pool       pool
);

void 
hemp_pool_free(
    hemp_pool       pool
);

void
hemp_pool_each(
    hemp_pool       pool,
    hemp_pool_iter  func
);


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_pool_new(size, capacity, cleaner)          \
    hemp_pool_init(NULL, size, capacity, cleaner)


#endif /* HEMP_POOL_H */

