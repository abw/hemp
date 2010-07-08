#ifndef HEMP_HASH_H
#define HEMP_HASH_H

#include <string.h>
#include "hemp/memory.h"
#include "hemp/text.h"

#define HEMP_HASH_DENSITY  5    /* max entries/width before increasing width */

typedef struct hemp_hash       * hemp_hash_t;
typedef struct hemp_hash_entry * hemp_hash_entry_t;
typedef hemp_size_t           (* hemp_hash_fn)(hemp_text_t); 

struct hemp_hash {
    hemp_hash_fn        hasher;   /* hash function                                */
    hemp_size_t         width;    /* number of columns, aka buckets, chains, etc. */
    hemp_size_t         size;     /* total number of entries                      */
    hemp_hash_entry_t   *columns;  /* array of pointers to head of each column     */
    hemp_hash_t          parent;   /* parent hash                                  */
}; 

struct hemp_hash_entry {
    hemp_size_t         hash;
    hemp_text_t         key;
    hemp_ptr_t          value;
    hemp_hash_entry_t   next;
};

hemp_hash_t         hemp_hash_init();
void                hemp_hash_free(hemp_hash_t);
void                hemp_hash_set_parent(hemp_hash_t, hemp_hash_t);
hemp_size_t         hemp_hash_resize(hemp_hash_t);
hemp_hash_entry_t   hemp_hash_store(hemp_hash_t, hemp_text_t, hemp_ptr_t);
hemp_ptr_t          hemp_hash_fetch(hemp_hash_t, hemp_text_t);
hemp_hash_t         hemp_hash_get_parent(hemp_hash_t);
hemp_text_t         hemp_hash_as_text(hemp_hash_t);
hemp_size_t         hemp_hash_function(hemp_text_t);
hemp_size_t         hemp_hash_function_jenkins32(hemp_text_t);

// for testing
void hemp_hash_print(hemp_hash_t);

//HASH hemp_hash_from_text(TEXT);

#endif /* HEMP_HASH_H */
