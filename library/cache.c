#include <hemp/cache.h>
#include <hemp/pool.h>


/*--------------------------------------------------------------------------
 * constructor/destructor functions
 *--------------------------------------------------------------------------*/

hemp_cache
hemp_cache_new(
    hemp_hemp hemp
) {
    hemp_cache cache;
    HEMP_ALLOCATE(cache);

    cache->hemp     = hemp;
    cache->fetch    = &hemp_cache_no_fetch;
    cache->store    = &hemp_cache_no_store;
    cache->delete   = &hemp_cache_no_delete;
    cache->empty    = &hemp_cache_no_empty;
    cache->cleanup  = NULL;
    cache->instance = NULL;

    return cache;
}


void
hemp_cache_free(
    hemp_cache cache
) {
    if (cache->cleanup)
        cache->cleanup(cache);

    hemp_mem_free(cache);
}


/*--------------------------------------------------------------------------
 * Default handler methods that silently do nothing
 *--------------------------------------------------------------------------*/

hemp_value 
hemp_cache_no_fetch(
    hemp_cache  cache,
    hemp_string key
) {
    hemp_fatal("fetch method is not defined for this cache\n");
    return HempMissing;
}

void
hemp_cache_no_store(
    hemp_cache  cache,
    hemp_string key,
    hemp_value  value
) {
    hemp_fatal("store method is not defined for this cache\n");
    /* do nothing */
}


void
hemp_cache_no_delete(
    hemp_cache  cache,
    hemp_string key
) {
    hemp_fatal("delete method is not defined for this cache\n");
    /* do nothing */
}

void
hemp_cache_no_empty(
    hemp_cache  cache
) {
    hemp_fatal("empty method is not defined for this cache\n");
    /* do nothing */
}


/*--------------------------------------------------------------------------
 * Default Least-Recently-Used (LRU) cache which caches up to a maximum
 * number of items, discarding those that were used least recently.
 *--------------------------------------------------------------------------*/


hemp_cache
hemp_cache_lru_new(
    hemp_hemp   hemp,
    hemp_size   size
) {
    hemp_cache      cache = hemp_cache_new(hemp);
    hemp_cache_lru  lru   = hemp_mem_alloc( sizeof(struct hemp_cache_lru) );
    hemp_size       min_size;

    if (! lru)
        hemp_mem_fail("LRU cache");

    if (size) {
        min_size = size < HEMP_CACHE_SIZE
            ? size
            : HEMP_CACHE_SIZE;
    }
    else {
        min_size = HEMP_CACHE_SIZE;
    }

    lru->size   = size;
    lru->index  = hemp_hash_new();
    lru->fresh  = NULL;
    lru->stale  = NULL;
    lru->unused = NULL;
    lru->pool   = hemp_pool_new(
        sizeof(struct hemp_cache_lru_slot), 
        min_size,
        NULL
    );

    cache->instance = lru;
    cache->fetch    = &hemp_cache_lru_fetch;
    cache->store    = &hemp_cache_lru_store;
    cache->delete   = &hemp_cache_lru_delete;
    cache->empty    = &hemp_cache_lru_empty;
    cache->cleanup  = &hemp_cache_lru_cleanup;

    return cache;

}

hemp_value
hemp_cache_lru_fetch(
    hemp_cache  cache,
    hemp_string key
) {
    hemp_debug_call("LRU fetch: %s\n", key);
    hemp_cache_lru  lru = (hemp_cache_lru) cache->instance;
    hemp_value      lookup;
    hemp_cache_lru_slot slot = NULL;

    lookup = hemp_hash_fetch(lru->index, key);

    if (hemp_is_found(lookup)) {
        /* found an existing cache entry */
        slot = hemp_val_ptr(lookup);
        hemp_cache_lru_reuse_slot(lru, slot);
    }

    return slot
        ? slot->value
        : HempMissing;
}


void
hemp_cache_lru_store(
    hemp_cache  cache,
    hemp_string key,
    hemp_value  value
) {
    hemp_debug_call("LRU store: %p\n", cache);
    hemp_cache_lru  lru = (hemp_cache_lru) cache->instance;
    hemp_value      lookup;
    hemp_cache_lru_slot slot;

    lookup = hemp_hash_fetch(lru->index, key);

    if (hemp_is_found(lookup)) {
        /* found an existing cache entry */
// TODO: check if the value is the same, otherwise free old value
// and replace with new
//      hemp_debug_msg("%s is already in cache\n", key);
        slot = hemp_val_ptr(lookup);
        hemp_cache_lru_reuse_slot(lru, slot);
    }
    else {
        /* need a new cache entry */
//      hemp_debug_msg("adding %s to cache\n", key);
        slot        = hemp_cache_lru_use_slot(lru);
        slot->key   = key;
        slot->value = value;
        hemp_hash_store(lru->index, key, hemp_ptr_val(slot));
    }
}


void
hemp_cache_lru_delete(
    hemp_cache  cache,
    hemp_string key
) {
    hemp_debug_call("LRU delete: %s\n", key);
    hemp_cache_lru      lru = (hemp_cache_lru) cache->instance;
    hemp_cache_lru_slot slot;
    hemp_value          value;

    value = hemp_hash_delete(lru->index, key);

    if (hemp_is_found(value)) {
        /* found an existing cache entry */
        slot = hemp_val_ptr(value);
        hemp_value_free(slot->value);
        hemp_cache_lru_unused_slot(lru, slot);
    }
}


void
hemp_cache_lru_empty(
    hemp_cache  cache
) {
    hemp_debug_call("LRU empty\n");
    hemp_cache_lru      lru  = (hemp_cache_lru) cache->instance;
    hemp_cache_lru_slot slot = lru->fresh;

    while (slot) {
//      hemp_debug_msg("emptying %s slot in cache\n", slot->key);
        hemp_value_free(slot->value);
        slot->value = HempMissing;

        if (slot->staler) {
            slot = slot->staler;
        }
        else {
            /* attach the last slot to the head of the unused slots list */
            slot->staler = lru->unused;
            lru->unused  = slot;
            break;
        }
    }
}


void
hemp_cache_lru_cleanup(
    hemp_cache  cache
) {
    hemp_cache_lru lru = (hemp_cache_lru) cache->instance;

    hemp_debug_call("cleaning cache/LRU instance: %p/%p\n", cache, lru);

    cache->empty(cache);
    hemp_pool_free(lru->pool);
    hemp_hash_free(lru->index);
    hemp_mem_free(lru);
}


void
hemp_cache_lru_debug(
    hemp_cache_lru lru
) {
    hemp_cache_lru_slot fresh = lru->fresh;
    hemp_cache_lru_slot stale = lru->stale;

    hemp_size n = 1;
    printf("LRU freshest to stalest / stalest to freshest\n");
    while (fresh && stale) {
        printf("%3ld: %-10s  %3ld: %-10s\n", n, fresh->key, -n, stale->key);
        n++;
        fresh = fresh->staler;
        stale = stale->fresher;
    }
}



/*--------------------------------------------------------------------------
 * Slot management functions for LRU cache
 *--------------------------------------------------------------------------*/

hemp_cache_lru_slot
hemp_cache_lru_use_slot(
    hemp_cache_lru lru
) {
    hemp_cache_lru_slot slot;

    if (lru->unused) {
        /* there's an unused slot available */
//      hemp_debug_msg("LRU has unused slot\n");
        slot = lru->unused;
        lru->unused = slot->staler;
    }
    else if (lru->size && lru->pool->used >= lru->size) {
        /* we've reached our capacity, so re-use an existing slot */
        if (! lru->stale)
            hemp_fatal("No stale slot to use\n");

//      hemp_debug_msg("LRU cache slots are full (%d), reusing oldest slot %s\n", lru->size, lru->stale->key);

        /* take the most stale slot and update the stale end of the list */
        slot = lru->stale;
        lru->stale = slot->fresher;
        
        if (slot->fresher)
            slot->fresher->staler = NULL;

        /* free the value and delete the entry from the index hash */
        hemp_value_free(slot->value);
        hemp_hash_delete(lru->index, slot->key);
    }
    else {
        /* otherwise take a new slot from the pool */
//      hemp_debug_msg("Using new LRU cache slot\n");
        slot = (hemp_cache_lru_slot) hemp_pool_take(lru->pool);
    }

    /* insert slot as the freshest item at the head of the list */
    hemp_cache_lru_freshen_slot(lru, slot);

    return slot;
}


void
hemp_cache_lru_reuse_slot(
    hemp_cache_lru      lru,
    hemp_cache_lru_slot slot
) {
    if (lru->fresh == slot)
        return;

    /* have links either side bypass this slot */
    if (slot->fresher)
        slot->fresher->staler = slot->staler;
    else
        lru->fresh = slot->staler;

    if (slot->staler)
        slot->staler->fresher = slot->fresher;
    else 
        lru->stale = slot->fresher;     /* slot was the stalest */

    /* insert slot as the freshest item at the head of the list */
    hemp_cache_lru_freshen_slot(lru, slot);
}


void
hemp_cache_lru_freshen_slot(
    hemp_cache_lru      lru,
    hemp_cache_lru_slot slot
) {
    /* existing freshest item now points to new item as being fresher */
    if (lru->fresh)
        lru->fresh->fresher = slot;

    /* new slot has nothing fresher, and the previous freshest as staler */
    slot->fresher = NULL;
    slot->staler = lru->fresh;
    
    /* lru point to new slot as freshest... */
    lru->fresh = slot;

    /* ... and also the stalest, if it's the first slot */
    if (! lru->stale)
        lru->stale = slot;
}


void
hemp_cache_lru_unused_slot(
    hemp_cache_lru      lru,
    hemp_cache_lru_slot slot
) {
    /* have links either side bypass this slot */
    if (slot->fresher)
        slot->fresher->staler = slot->staler;
    else
        lru->fresh = slot->staler;

    if (slot->staler)
        slot->staler->fresher = slot->fresher;
    else 
        lru->stale = slot->fresher;     /* slot was the stalest */

    slot->staler = lru->unused;
    lru->unused  = slot;
}


