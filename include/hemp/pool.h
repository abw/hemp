#ifndef HEMP_POOL_H
#define HEMP_POOL_H

#include <hemp/slab.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_pool_s {
    hemp_slab_p         slab;           /* pointer to slab(s) of memory     */
    hemp_mem_p          next;           /* next available memory record     */
    hemp_size_t         size;           /* size of each memory record       */
    hemp_size_t         used;           /* number of records in use         */
    hemp_size_t         capacity;       /* allocated storage capacity       */
    hemp_pool_each_f    cleaner;        /* memory cleanup function          */
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_pool_p
hemp_pool_init(
    hemp_pool_p         pool,
    hemp_size_t         size,
    hemp_size_t         capacity,
    hemp_pool_each_f    cleaner
);

hemp_mem_p
hemp_pool_take(
    hemp_pool_p         pool
);

void 
hemp_pool_grow(
    hemp_pool_p         pool
);

void 
hemp_pool_free(
    hemp_pool_p         pool
);

void
hemp_pool_each(
    hemp_pool_p         pool,
    hemp_pool_each_f    func
);


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_pool_new(size, capacity, cleaner)          \
    hemp_pool_init(NULL, size, capacity, cleaner)

#define hemp_pool_allocate(name)                        \
    hemp_allocate_type_name(pool, name)


#endif /* HEMP_POOL_H */

