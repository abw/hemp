#ifndef HEMP_HASH_H
#define HEMP_HASH_H

#include <hemp/core.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_hash_s {
    hemp_size_t         width;      /* number of columns        */
    hemp_size_t         size;       /* total number of entries  */
    hemp_hash_item_p  * columns;    /* column heads             */
    hemp_hash_p         parent;     /* parent hash              */
}; 

struct hemp_hash_item_s {
    hemp_size_t         hash;       /* computed hash value      */
    hemp_cstr_p         key;        /* lookup key               */
    hemp_mem_p          value;      /* corresponding value      */
    hemp_hash_item_p    next;       /* next item in column      */
};


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_hash_p
    hemp_hash_init();

void
    hemp_hash_free(
        hemp_hash_p hash
    );

void
    hemp_hash_attach(
        hemp_hash_p child, 
        hemp_hash_p parent
    );

void
    hemp_hash_detach(
        hemp_hash_p child
    );

hemp_size_t
    hemp_hash_resize(
        hemp_hash_p hash
    );

hemp_hash_item_p
    hemp_hash_store(
        hemp_hash_p hash, 
        hemp_cstr_p key, 
        hemp_mem_p  value
    );

hemp_mem_p
    hemp_hash_fetch(
        hemp_hash_p hash, 
        hemp_cstr_p key
    );

hemp_cstr_p
    hemp_hash_as_cstr(
        hemp_hash_p hash
    );

hemp_size_t
    hemp_hash_function_default(
        hemp_cstr_p key
    );

hemp_size_t
    hemp_hash_function_jenkins32(
        hemp_cstr_p key
    );

void
    hemp_hash_each(
        hemp_hash_p      table, 
        hemp_hash_each_f func
    );

// for testing
void 
    hemp_hash_print(
        hemp_hash_p hash
    );


/*--------------------------------------------------------------------------
 * patch in the hash function we want to use
 *--------------------------------------------------------------------------*/

#define hemp_hash_function(cstr)    \
    hemp_hash_function_jenkins32(cstr)


#endif /* HEMP_HASH_H */
