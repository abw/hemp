/* A pool performs block memory allocation for multiple records of a fixed 
 * size.  It stores the head of a linked list of memory slabs and bits of 
 * housekeeping data including the records size, number of records currently 
 * in use and the record capacity for which memory has been pre-allocated.
 */

#include <hemp/pool.h>


hemp_pool
hemp_pool_init(
    hemp_pool       pool, 
    hemp_size       size, 
    hemp_size       capacity,
    hemp_pool_iter  cleaner
) {
    HEMP_INSTANCE(pool);

    pool->slab     = hemp_slab_new(size * capacity);
    pool->next     = pool->slab->data;
    pool->size     = size;
    pool->capacity = capacity;
    pool->used     = 0;
    pool->cleaner  = cleaner;

/*
    hemp_debug_mem(
        "Allocated pool buffer of %d x %d = %d bytes\n", 
        size, capacity, size * capacity
    );
    hemp_debug_mem(
        "pool slab data is from %p to %p\n", 
        pool->next, pool->next + size * capacity
    );
*/
    
    return pool;
}


HEMP_INLINE hemp_memory
hemp_pool_take(
    hemp_pool pool
) {
    hemp_memory value;

//  hemp_debug_msg("taking from pool at %p with slab at %p\n", pool, pool->slab);

    if (pool->used == pool->capacity)
        hemp_pool_grow(pool);

//  hemp_debug_msg("Returning pool item %d at %p\n", pool->used, pool->next);

    value = pool->next;
    pool->next += pool->size;
    pool->used++;
    return value;
}


HEMP_INLINE void
hemp_pool_give(
    hemp_pool   pool,
    hemp_memory item
) {
    hemp_debug_mem("giving %p to pool at %p with slab at %p\n", item, pool, pool->slab);

    /* This hasn't been tested yet... I implemented it and then realised 
     * that it wasn't going to be suitable for the use I had in mind, so 
     * it lies semi-abandonded.  The difficulty in restored returning items 
     * across slab boundaries and/or out of order makes me thing we would 
     * be better off with a different data structure.  So it's on the back
     * burner for now.
     */

    if (pool->next == item + pool->size) {
        /* The simple case where the item returned is the last one taken,
         * immediately preceding the next item in memory.  We call any 
         * cleanup function and then update the next/used values
         */
        hemp_debug_msg("item returned to pool is the last item taken\n");

        if (pool->cleaner)
            pool->cleaner(item);
        
        pool->used--;
        pool->next = item;
    }
    else {
        /* Otherwise we'll barf for now... I don't have a pressing need (yet)
         * to use this for anything other than a strict last-out, first-in 
         * stack for scanning nested tags, which the above case handles.
         */
        hemp_fatal("Unable to return item to pool (that functionality is TODO)\n");
    }
}


HEMP_INLINE void 
hemp_pool_grow(
    hemp_pool pool
) {
    /* 
    ** create a new slab as large as our current capacity (so the pool size
    ** doubles on each growth) and insert it at the head of the slab list
    */
//  hemp_debug_mem("growing pool at %p with slab at %p\n", pool, pool->slab);
    hemp_slab slab  = hemp_slab_new(pool->size * pool->capacity);
    slab->next      = pool->slab;
    pool->slab      = slab;
    pool->next      = slab->data;
    pool->capacity *= 2;
//  hemp_debug_mem("Increased pool capacity to %d\n", pool->capacity);
}


void
hemp_pool_free(
    hemp_pool pool
) {
//  hemp_debug_mem("Releasing pool at %p with slab at %p\n", pool, pool->slab);

    if (pool->cleaner)
        hemp_pool_each(pool, pool->cleaner);

    if (pool->slab)
        hemp_slab_free(pool->slab);

    hemp_mem_free(pool);
}


void
hemp_pool_each(
    hemp_pool       pool,
    hemp_pool_iter  func
) {
    hemp_slab   slab = pool->slab;
    hemp_memory item;
    hemp_size   size;
    
    while (slab) {
        item = slab->data;
        size = slab->size / pool->size;
        while (size-- && item != pool->next) {
            /* call function, if it returns true then continue */
            if (! func(item)) {
                return;
            }
            item += pool->size;
        }
        slab = slab->next;
    }
}
