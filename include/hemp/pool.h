#ifndef HEMP_POOL_H
#define HEMP_POOL_H

#include <hemp/slab.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_pool {
    HempSlab        slab;           /* pointer to slab(s) of memory         */
    HempMemory      next;           /* next available memory record         */
    HempSize        size;           /* size of each memory record           */
    HempSize        used;           /* number of records in use             */
    HempSize        capacity;       /* allocated storage capacity           */
    hemp_pool_iter  cleaner;        /* memory cleanup function              */
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempPool
hemp_pool_init(
    HempPool        pool,
    HempSize        size,
    HempSize        capacity,
    hemp_pool_iter  cleaner
);

HempMemory
hemp_pool_take(
    HempPool        pool
);

void
hemp_pool_give(
    HempPool        pool,
    HempMemory      item
);

void
hemp_pool_grow(
    HempPool        pool
);

void
hemp_pool_free(
    HempPool        pool
);

void
hemp_pool_each(
    HempPool        pool,
    hemp_pool_iter  func
);


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_pool_new(size, capacity, cleaner)          \
    hemp_pool_init(NULL, size, capacity, cleaner)


#endif /* HEMP_POOL_H */
