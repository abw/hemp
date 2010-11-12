/* A pool performs block memory allocation for multiple records of a fixed 
 * size.  It stores the head of a linked list of memory slabs and bits of 
 * housekeeping data including the records size, number of records currently 
 * in use and the record capacity for which memory has been pre-allocated.
 */

#include <hemp/pool.h>


hemp_pool_p
hemp_pool_init(
    hemp_size_t size, 
    hemp_size_t capacity
) {
    hemp_pool_p pool = (hemp_pool_p) hemp_mem_alloc(
        sizeof(struct hemp_pool_s)
    );

    if (! pool)
        hemp_mem_fail("pool");

    pool->slab     = hemp_slab_init(size * capacity);
    pool->next     = pool->slab->data;
    pool->size     = size;
    pool->capacity = capacity;
    pool->used     = 0;

//  hemp_debug_mem("pool slab data is from %p to %p\n", pool->next, pool->next + size * capacity);
    /*
    hemp_debug_mem(
        "Allocated pool buffer of %d x %d = %d bytes\n", 
        size, capacity, size * capacity
    );]
    */
    
    return pool;
}


hemp_mem_p
hemp_pool_take(
    hemp_pool_p pool
) {
    hemp_mem_p value;

//  hemp_debug_mem("taking from pool at %p with slab at %p\n", pool, pool->slab);

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
    hemp_pool_p pool
) {
    // create a new slab as large as our current capacity (so the pool size
    // doubles on each growth) and insert it at the head of the slab list
//    hemp_debug_mem("growing pool at %p with slab at %p\n", pool, pool->slab);
    hemp_slab_p slab = hemp_slab_init(pool->size * pool->capacity);
    slab->next = pool->slab;
    pool->slab = slab;
    pool->next = slab->data;
    pool->capacity *= 2;
//    hemp_debug_mem("Increased pool capacity to %d\n", pool->capacity);
}


void
hemp_pool_free(
    hemp_pool_p pool
) {
//    hemp_debug_mem("Releasing pool at %p with slab at %p\n", pool, pool->slab);

    if (pool->slab)
        hemp_slab_free(pool->slab);

    hemp_mem_free(pool);
}

void
hemp_pool_each(
    hemp_pool_p      pool,
    hemp_pool_each_f func
) {
    hemp_slab_p slab = pool->slab;
    hemp_mem_p  item;
    hemp_size_t size;
    
    while (slab) {
        item = slab->data;
        size = slab->size / pool->size;
//        hemp_debug("visiting slab (size: %d) at %p\n", size, slab); 
        while (size-- && item != pool->next) {
//            hemp_debug("visiting item at %p\n", item);
            if (! func(item)) {
                return;
            }
            item += pool->size;
        }
        slab = slab->next;
    }
}