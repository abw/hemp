#ifndef HEMP_HASH_H
#define HEMP_HASH_H

#include <hemp/slot.h>


/*--------------------------------------------------------------------------
 * data structures
 *--------------------------------------------------------------------------*/

struct hemp_hash_s {
    hemp_size_t         width;      /* number of columns        */
    hemp_size_t         size;       /* total number of entries  */
    hemp_slot_p       * slots;      /* heads of slot columns    */
    hemp_hash_p         parent;     /* parent hash              */
}; 

// old hash item
// struct hemp_hash_item_s {
//    hemp_size_t         hash;       /* computed hash value      */
//    hemp_cstr_p         key;        /* lookup key               */
//    hemp_mem_p          value;      /* corresponding value      */
//    hemp_hash_item_p    next;       /* next item in column      */
//};

// new generic slot
//struct hemp_slot_s {
//    hemp_value_t        parent;     /* owner of this slot                   */
//    hemp_size_t         index;      /* numerical hash/index value           */
//    hemp_cstr_p         name;       /* lookup key                           */
//    hemp_value_t        value;      /* corresponding value                  */
//    hemp_slot_p         next;       /* next slot in hash/free linked list   */
//};



/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_hash_p
    hemp_hash_init();

void
    hemp_hash_free(
        hemp_hash_p     hash
    );

void
    hemp_hash_attach(
        hemp_hash_p     child, 
        hemp_hash_p     parent
    );

void
    hemp_hash_detach(
        hemp_hash_p     child
    );

hemp_size_t
    hemp_hash_resize(
        hemp_hash_p     hash
    );

hemp_slot_p
    hemp_hash_store(
        hemp_hash_p     hash, 
        hemp_cstr_p     key, 
        hemp_value_t    value
    );

hemp_value_t
    hemp_hash_fetch(
        hemp_hash_p     hash, 
        hemp_cstr_p     key
    );

hemp_mem_p
    hemp_hash_fetch_pointer(
        hemp_hash_p     hash, 
        hemp_cstr_p     key
    );

hemp_cstr_p
    hemp_hash_fetch_string(
        hemp_hash_p     hash, 
        hemp_cstr_p     key
    );

hemp_cstr_p
    hemp_hash_as_cstr(
        hemp_hash_p hash
    );

hemp_size_t
    hemp_hash_function_default(
        hemp_cstr_p key
    );

HEMP_DO_INLINE hemp_size_t
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
 * patch in the hash function we want to use and define some other macros
 *--------------------------------------------------------------------------*/

#define hemp_hash_function(cstr)        \
    hemp_hash_function_jenkins32(cstr)

#define hemp_hash_store_number(h,k,n)   \
    hemp_hash_store(h, k, hemp_num_val(n))

#define hemp_hash_store_integer(h,k,i)  \
    hemp_hash_store(h, k, hemp_int_val(i))

#define hemp_hash_store_pointer(h,k,p)  \
    hemp_hash_store(h, k, hemp_ptr_val(p))

#define hemp_hash_store_string(h,k,s)   \
    hemp_hash_store(h, k, hemp_str_val(s))

#endif /* HEMP_HASH_H */
