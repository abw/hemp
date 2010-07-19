/* A pool performs block memory allocation for multiple records of a fixed 
 * size.  It stores the head of a linked list of memory slabs and bits of 
 * housekeeping data including the records size, number of records currently 
 * in use and the record capacity for which memory has been pre-allocated.
 */

#include "hemp/pool.h"
#include "hemp/debug.h"



hemp_type_t
hemp_pool_type() {
    // we should eventually be calling hemp_new(Type, "Pool", $bigger_size)
    // to create a new generic Pool type from which we can derive specific
    // pools... or rather, we want to create a new type that pools objects
    // between calls to acquire() and release()
    hemp_type_t Pool = hemp_type_init("Pool", sizeof(struct hemp_pool));
    Pool->prepare = &hemp_pool_prepare;
    Pool->cleanup = &hemp_pool_cleanup;
    return Pool;
}


/* new methods that are invoked via hemp_new(Pool) */

hemp_bool_t 
hemp_pool_prepare(
    hemp_pool_t pool,
    HEMP_ARGS
) {
    hemp_size_t size     = HEMP_ARG(hemp_size_t);
    hemp_size_t capacity = HEMP_ARG(hemp_size_t);

    pool->slab      = hemp_slab_init(size * capacity);
    pool->next      = pool->slab->data;
    pool->size      = size;
    pool->capacity  = capacity;
    pool->used      = 0;

    debug_mem(
        "Allocated pool buffer of %d x %d = %d bytes\n", 
        size, capacity, size * capacity
    );

    return HEMP_TRUE;
}


hemp_bool_t
hemp_pool_cleanup(
    hemp_pool_t pool
) {
    debug_mem("Cleaning pool at %p\n", pool);

    if (pool->slab)
        hemp_slab_free(pool->slab);

    return HEMP_TRUE;
}


/* old methods that create/destory a pool directly */
hemp_pool_t 
hemp_pool_init(
    hemp_size_t size, 
    hemp_size_t capacity
) {
    hemp_pool_t pool;

    if ((pool = (hemp_pool_t) hemp_mem_init(sizeof(struct hemp_pool)))) {
        if ((pool->slab    = hemp_slab_init(size * capacity))) {
            pool->next     = pool->slab->data;
            pool->size     = size;
            pool->capacity = capacity;
            pool->used     = 0;
            debug_mem(
                "Allocated pool buffer of %d x %d = %d bytes\n", 
                size, capacity, size * capacity
            );
        }
        else {
            hemp_pool_null(pool);
        }
    }
    
    // TODO: handle pool == NULL
    
    return pool;
}


hemp_ptr_t
hemp_pool_take(hemp_pool_t pool) {
    hemp_ptr_t value;

    if (pool->used == pool->capacity)
        hemp_pool_grow(pool);

    // debug_cyan("Returning pool item %d at %p\n", pool->used, pool->next);

    value = pool->next;
    pool->next += pool->size;
    pool->used++;
    return value;
}


void 
hemp_pool_grow(
    hemp_pool_t pool
) {
    // create a new slab as large as our current capacity (so the pool size
    // doubles on each growth) and insert it at the head of the slab list
    hemp_slab_t slab = hemp_slab_init(pool->size * pool->capacity);
    slab->next = pool->slab;
    pool->slab = slab;
    pool->next = slab->data;
    pool->capacity *= 2;
    debug_mem("Increased pool capacity to %d\n", pool->capacity);
}


void
hemp_pool_free(
    hemp_pool_t pool
) {
    debug_mem("Releasing pool at %p\n", pool);

    if (pool->slab)
        hemp_slab_free(pool->slab);

    hemp_mem_free(pool);
}
