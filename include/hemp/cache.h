#ifndef HEMP_CACHE_H
#define HEMP_CACHE_H

#include <hemp/core.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

typedef hemp_value
(* hemp_cache_fetch)(
    hemp_cache  cache,
    hemp_string key
);

typedef void
(* hemp_cache_store)(
    hemp_cache  cache,
    hemp_string key,
    hemp_value  value
);

typedef void
(* hemp_cache_delete)(
    hemp_cache  cache,
    hemp_string key
);

typedef void
(* hemp_cache_empty)(
    hemp_cache  cache
);

typedef void
(* hemp_cache_cleanup)(
    hemp_cache  cache
);


struct hemp_cache {
    hemp_hemp           hemp;
    hemp_cache_fetch    fetch;
    hemp_cache_store    store;
    hemp_cache_delete   delete;
    hemp_cache_empty    empty;
    hemp_cache_cleanup  cleanup;
    hemp_memory         instance;
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_cache
hemp_cache_new(
    hemp_hemp       hemp
);

void
hemp_cache_free(
    hemp_cache      cache
);


hemp_value 
hemp_cache_no_fetch(
    hemp_cache  cache,
    hemp_string key
);

void
hemp_cache_no_store(
    hemp_cache  cache,
    hemp_string key,
    hemp_value  value
);

void
hemp_cache_no_delete(
    hemp_cache  cache,
    hemp_string key
);

void
hemp_cache_no_empty(
    hemp_cache  cache
);


/*--------------------------------------------------------------------------
 * Default LRU cache.
 *--------------------------------------------------------------------------*/

#define HEMP_CACHE_LRU_SIZE         16

typedef struct hemp_cache_lru      * hemp_cache_lru;
typedef struct hemp_cache_lru_slot * hemp_cache_lru_slot;

struct hemp_cache_lru {
    hemp_size           size;
    hemp_hash           index;
    hemp_pool           pool;
    hemp_cache_lru_slot fresh;      /* freshest (most recently used) slot   */
    hemp_cache_lru_slot stale;      /* stalest (least recently used) slot   */
    hemp_cache_lru_slot unused;     /* unused slots (typically post-delete) */
};

struct hemp_cache_lru_slot {
    hemp_string         key;
    hemp_value          value;
    hemp_cache_lru_slot fresher;    /* previous (more recently used) slot   */
    hemp_cache_lru_slot staler;     /* next (less recently used) slot       */
};


hemp_cache
hemp_cache_lru_new(
    hemp_hemp   hemp,
    hemp_size   size
);

hemp_value 
hemp_cache_lru_fetch(
    hemp_cache  cache,
    hemp_string key
);

void
hemp_cache_lru_store(
    hemp_cache  cache,
    hemp_string key,
    hemp_value  value
);

void
hemp_cache_lru_delete(
    hemp_cache  cache,
    hemp_string key
);

void
hemp_cache_lru_empty(
    hemp_cache  cache
);

void
hemp_cache_lru_cleanup(
    hemp_cache  cache
);

void
hemp_cache_lru_debug(
    hemp_cache_lru lru
);

hemp_cache_lru_slot
hemp_cache_lru_use_slot(
    hemp_cache_lru lru
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


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/


#endif /* HEMP_CACHE_H */
