#ifndef HEMP_HASH_H
#define HEMP_HASH_H

#include <string.h>
#include "hemp/memory.h"
#include "hemp/cstr.h"
#include "hemp/debug.h"

#define HEMP_HASH_DENSITY  5    /* max entries/width before increasing width */


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_hash {
    hemp_hash_fn        hasher;     /* hash function            */
    hemp_size_t         width;      /* number of columns        */
    hemp_size_t         size;       /* total number of entries  */
    hemp_hash_entry_t   *columns;   /* column heads             */
    hemp_hash_t          parent;    /* parent hash              */
}; 

struct hemp_hash_entry {
    hemp_size_t         hash;       /* computed hash value      */
    hemp_cstr_t         key;        /* lookup key               */
    hemp_ptr_t          value;      /* corresponding value      */
    hemp_hash_entry_t   next;       /* next entry in column     */
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_hash_t
    hemp_hash_init();

void
    hemp_hash_free(
        hemp_hash_t hash
    );

void
    hemp_hash_set_parent(
        hemp_hash_t child, 
        hemp_hash_t parent
    );

hemp_size_t
    hemp_hash_resize(
        hemp_hash_t hash
    );

hemp_hash_entry_t
    hemp_hash_store(
        hemp_hash_t hash, 
        hemp_cstr_t key, 
        hemp_ptr_t  value
    );

hemp_ptr_t
    hemp_hash_fetch(
        hemp_hash_t hash, 
        hemp_cstr_t key
    );

hemp_hash_t
    hemp_hash_get_parent(
        hemp_hash_t hash
    );

hemp_cstr_t
    hemp_hash_as_text(
        hemp_hash_t hash
    );

hemp_size_t
    hemp_hash_function(
        hemp_cstr_t key
    );

hemp_size_t
    hemp_hash_function_jenkins32(
        hemp_cstr_t key
    );

void
    hemp_hash_each(
        hemp_hash_t table, 
        void (* callback)(hemp_hash_entry_t)
    );

// for testing
void 
    hemp_hash_print(
        hemp_hash_t hash
    );


//HASH hemp_hash_from_text(TEXT);


/*--------------------------------------------------------------------------
 * macros
 *--------------------------------------------------------------------------*/

#define hemp_hash_null(h) \
    hemp_hash_free(h);    \
    h = NULL;


#endif /* HEMP_HASH_H */
