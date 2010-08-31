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
//    hemp_str_p         key;        /* lookup key               */
//    hemp_mem_p          value;      /* corresponding value      */
//    hemp_hash_item_p    next;       /* next item in column      */
//};

// new generic slot
//struct hemp_slot_s {
//    hemp_value_t        parent;     /* owner of this slot                   */
//    hemp_size_t         index;      /* numerical hash/index value           */
//    hemp_str_p         name;       /* lookup key                           */
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
        hemp_str_p      key, 
        hemp_value_t    value
    );

HEMP_INLINE hemp_bool_t 
    hemp_hash_key_match(
        hemp_str_p  key1,
        hemp_str_p  key2,
        hemp_size_t length
    );

HEMP_INLINE hemp_value_t
    hemp_hash_fetch_keylen(
        hemp_hash_p hash, 
        hemp_str_p  name,
        hemp_size_t length
    );

HEMP_INLINE hemp_value_t
    hemp_hash_fetch(
        hemp_hash_p hash, 
        hemp_str_p  key
    );

HEMP_INLINE hemp_num_t
    hemp_hash_fetch_number(
        hemp_hash_p hash, 
        hemp_str_p  name
    );

HEMP_INLINE hemp_int_t
    hemp_hash_fetch_integer(
        hemp_hash_p hash, 
        hemp_str_p  name
    );

HEMP_INLINE hemp_mem_p
    hemp_hash_fetch_pointer(
        hemp_hash_p hash, 
        hemp_str_p  key
    );

HEMP_INLINE hemp_str_p
    hemp_hash_fetch_string(
        hemp_hash_p hash, 
        hemp_str_p  key
    );

HEMP_INLINE hemp_text_p
    hemp_hash_fetch_text(
        hemp_hash_p hash, 
        hemp_str_p  key
    );

hemp_str_p
    hemp_hash_as_string(
        hemp_hash_p hash
    );

hemp_size_t
    hemp_hash_function_default(
        hemp_str_p key
    );

HEMP_DO_INLINE hemp_size_t
    hemp_hash_function_jenkins32(
        hemp_str_p  key,
        hemp_size_t length
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
 * type functions and methods
 *--------------------------------------------------------------------------*/

HEMP_TYPE_FUNC(hemp_type_hash);
HEMP_VTEXT_FUNC(hemp_value_hash_text);
HEMP_VALUE_FUNC(hemp_method_hash_length);


/*--------------------------------------------------------------------------
 * patch in the hash function we want to use and define some other macros
 *--------------------------------------------------------------------------*/

#define hemp_hash_function(string, len)       \
    hemp_hash_function_jenkins32(string, len)

#define hemp_hash_store_number(h, k, n)     \
    hemp_hash_store(h, k, hemp_num_val(n))

#define hemp_hash_store_integer(h, k, i)    \
    hemp_hash_store(h, k, hemp_int_val(i))

#define hemp_hash_store_pointer(h, k, p)    \
    hemp_hash_store(h, k, hemp_ptr_val(p))

#define hemp_hash_store_string(h, k, s)     \
    hemp_hash_store(h, k, hemp_str_val(s))

#define hemp_hash_store_text(h, k, t)       \
    hemp_hash_store(h, k, hemp_text_val(t))

#endif /* HEMP_HASH_H */
