#ifndef HEMP_HASH_H
#define HEMP_HASH_H

#include <hemp/slot.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_hash {
    hemp_size       width;      /* number of columns        */
    hemp_size       size;       /* total number of entries  */
    hemp_slot     * slots;      /* heads of slot columns    */
    hemp_hash       parent;     /* parent hash              */
}; 


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

hemp_hash
hemp_hash_init();

HEMP_INLINE void
hemp_hash_release(
    hemp_hash       hash
);

void
hemp_hash_free(
    hemp_hash       hash
);

HEMP_INLINE void
hemp_hash_attach(
    hemp_hash       child,
    hemp_hash       parent
);

HEMP_INLINE void
hemp_hash_detach(
    hemp_hash       child
);

HEMP_INLINE hemp_size
hemp_hash_grow(
    hemp_hash       hash
);

HEMP_INLINE hemp_slot
hemp_hash_store_keylen(
    hemp_hash       hash,
    hemp_string     key,
    hemp_value      value,
    hemp_size       length
);

hemp_slot
hemp_hash_store(
    hemp_hash       hash,
    hemp_string     key,
    hemp_value      value
);


HEMP_INLINE hemp_bool 
hemp_hash_key_match(
    hemp_string     key1,
    hemp_string     key2,
    hemp_size       length
);

HEMP_INLINE hemp_value
hemp_hash_fetch_keylen(
    hemp_hash       hash,
    hemp_string     name,
    hemp_size       length
);

HEMP_INLINE hemp_value
hemp_hash_fetch(
    hemp_hash       hash,
    hemp_string     key
);

HEMP_INLINE hemp_num
hemp_hash_fetch_number(
    hemp_hash       hash,
    hemp_string     name
);

HEMP_INLINE hemp_int
hemp_hash_fetch_integer(
    hemp_hash       hash,
    hemp_string     name
);

HEMP_INLINE hemp_memory
hemp_hash_fetch_pointer(
    hemp_hash       hash,
    hemp_string     key
);

HEMP_INLINE hemp_string
hemp_hash_fetch_string(
    hemp_hash       hash,
    hemp_string     key
);

HEMP_INLINE hemp_text
hemp_hash_fetch_text(
    hemp_hash       hash,
    hemp_string     key
);

HEMP_INLINE hemp_list
hemp_hash_fetch_list(
    hemp_hash       hash,
    hemp_string     name
);

HEMP_INLINE hemp_hash
hemp_hash_fetch_hash(
    hemp_hash       hash,
    hemp_string     name
);

hemp_value
hemp_hash_find(
    hemp_hash       hash,
    hemp_string     key,
    hemp_context    context
);

hemp_string
hemp_hash_as_string(
    hemp_hash       hash
);

hemp_size
hemp_hash_function_default(
    hemp_string     key
);

HEMP_INLINE hemp_size
hemp_hash_function_jenkins32(
    hemp_string     key,
    hemp_size       length
);

void
hemp_hash_each(
    hemp_hash       hash,
    hemp_hash_iter  func
);

// for testing

hemp_text
hemp_hash_dump(
    hemp_hash       hash
);


/*--------------------------------------------------------------------------
 * type functions and methods
 *--------------------------------------------------------------------------*/

HEMP_TYPE_FUNC(hemp_type_hash);
HEMP_OUTPUT(hemp_type_hash_text);
HEMP_FETCH_FUNC(hemp_type_hash_fetch);
HEMP_STORE_FUNC(hemp_type_hash_store);
HEMP_FETCH_FUNC(hemp_type_hash_dot);
HEMP_OUTPUT(hemp_type_hash_pairs);
HEMP_VALUE(hemp_method_hash_length);


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

#define hemp_hash_store_list(h, k, l)       \
    hemp_hash_store(h, k, hemp_list_val(l))

#define hemp_hash_store_hash(h, k, v)       \
    hemp_hash_store(h, k, hemp_hash_val(v))

#define hemp_hash_new() \
    hemp_hash_init(NULL)


#endif /* HEMP_HASH_H */
