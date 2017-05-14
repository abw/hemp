#ifndef HEMP_CACHE_H
#define HEMP_CACHE_H

#include <hemp/core.h>
#include <hemp/type/hash.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

typedef HempValue
(* HempCacheFetch)(
    HempCache       cache,
    HempString      key
);

typedef void
(* HempCacheStore)(
    HempCache       cache,
    HempString      key,
    HempValue       value
);

typedef void
(* HempCacheDelete)(
    HempCache       cache,
    HempString      key
);

typedef void
(* HempCacheEmpty)(
    HempCache       cache
);

typedef void
(* HempCacheClean)(
    HempCache       cache
);


struct hemp_cache {
    Hemp            hemp;
    HempString      name;
    HempCacheFetch  fetch;
    HempCacheStore  store;
    HempCacheDelete delete;
    HempCacheEmpty  empty;
    HempCacheClean  clean;
    HempMemory      instance;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempCache
hemp_cache_new(
    Hemp            hemp
);

void
hemp_cache_free(
    HempCache       cache
);


HempValue
hemp_cache_no_fetch(
    HempCache       cache,
    HempString      key
);

void
hemp_cache_no_store(
    HempCache       cache,
    HempString      key,
    HempValue       value
);

void
hemp_cache_no_delete(
    HempCache       cache,
    HempString      key
);

void
hemp_cache_no_empty(
    HempCache       cache
);



/*--------------------------------------------------------------------------
 * Default LRU cache.
 *--------------------------------------------------------------------------*/

typedef struct hemp_cache_lru      * hemp_cache_lru;
typedef struct hemp_cache_lru_slot * hemp_cache_lru_slot;

struct hemp_cache_lru {
    HempSize            size;
    HempHash            index;
    HempPool            pool;
    hemp_cache_lru_slot fresh;      /* freshest (most recently used) slot   */
    hemp_cache_lru_slot stale;      /* stalest (least recently used) slot   */
    hemp_cache_lru_slot unused;     /* unused slots (typically post-delete) */
};

struct hemp_cache_lru_slot {
    HempString          key;
    HempValue           value;
    hemp_cache_lru_slot fresher;    /* previous (more recently used) slot   */
    hemp_cache_lru_slot staler;     /* next (less recently used) slot       */
};


HempCache
hemp_cache_lru_new(
    Hemp            hemp,
    HempSize        size
);

HempValue
hemp_cache_lru_fetch(
    HempCache       cache,
    HempString      key
);

void
hemp_cache_lru_store(
    HempCache       cache,
    HempString      key,
    HempValue       value
);

void
hemp_cache_lru_delete(
    HempCache       cache,
    HempString      key
);

void
hemp_cache_lru_empty(
    HempCache       cache
);

void
hemp_cache_lru_clean(
    HempCache       cache
);

void
hemp_cache_lru_debug(
    hemp_cache_lru  lru
);

hemp_cache_lru_slot
hemp_cache_lru_use_slot(
    hemp_cache_lru  lru
);

void
hemp_cache_lru_reuse_slot(
    hemp_cache_lru      lru,
    hemp_cache_lru_slot slot
);

void
hemp_cache_lru_unused_slot(
    hemp_cache_lru      lru,
    hemp_cache_lru_slot slot
);

void
hemp_cache_lru_freshen_slot(
    hemp_cache_lru      lru,
    hemp_cache_lru_slot slot
);


#endif /* HEMP_CACHE_H */
