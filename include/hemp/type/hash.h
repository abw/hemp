#ifndef HEMP_HASH_H
#define HEMP_HASH_H

#include <hemp/slot.h>


/*--------------------------------------------------------------------------
 * type definitions
 *--------------------------------------------------------------------------*/

struct hemp_hash {
    HempSize       width;      /* number of columns        */
    HempSize       size;       /* total number of entries  */
    HempSlot     * slots;      /* heads of slot columns    */
    HempHash       parent;     /* parent hash              */
}; 


/*--------------------------------------------------------------------------
 * function prototypes
 *--------------------------------------------------------------------------*/

HempHash
hemp_hash_init();

HEMP_INLINE void
hemp_hash_release(
    HempHash        hash
);

void
hemp_hash_free(
    HempHash        hash
);

HEMP_INLINE void
hemp_hash_attach(
    HempHash        child,
    HempHash        parent
);

HEMP_INLINE void
hemp_hash_detach(
    HempHash        child
);

HEMP_INLINE HempSize
hemp_hash_grow(
    HempHash        hash
);

HEMP_INLINE HempSlot
hemp_hash_store_keylen(
    HempHash        hash,
    HempString      key,
    HempValue       value,
    HempSize        length
);

HempSlot
hemp_hash_store(
    HempHash        hash,
    HempString      key,
    HempValue       value
);


HEMP_INLINE HempBool 
hemp_hash_key_match(
    HempString      key1,
    HempString      key2,
    HempSize        length
);

HEMP_INLINE HempValue
hemp_hash_fetch_keylen(
    HempHash        hash,
    HempString      name,
    HempSize        length
);

HEMP_INLINE HempValue
hemp_hash_fetch(
    HempHash        hash,
    HempString      key
);

HEMP_INLINE HempNum
hemp_hash_fetch_number(
    HempHash        hash,
    HempString      name
);

HEMP_INLINE HempInt
hemp_hash_fetch_integer(
    HempHash        hash,
    HempString      name
);

HEMP_INLINE HempMemory
hemp_hash_fetch_pointer(
    HempHash        hash,
    HempString      key
);

HEMP_INLINE HempString
hemp_hash_fetch_string(
    HempHash        hash,
    HempString      key
);

HEMP_INLINE HempText
hemp_hash_fetch_text(
    HempHash        hash,
    HempString      key
);

HEMP_INLINE HempList
hemp_hash_fetch_list(
    HempHash        hash,
    HempString      name
);

HEMP_INLINE HempHash
hemp_hash_fetch_hash(
    HempHash        hash,
    HempString      name
);

HempValue
hemp_hash_fetch_dotted(
    HempHash        hash,
    HempString      key,
    HempContext     context
);

HempSlot
hemp_hash_store_dotted(
    HempHash        hash,
    HempString      name,
    HempValue       value,
    HempContext     context
);

HEMP_INLINE HempValue
hemp_hash_delete(
    HempHash        hash,
    HempString      key
);

HempString
hemp_hash_as_string(
    HempHash        hash
);

HempSize
hemp_hash_function_default(
    HempString      key
);

HEMP_INLINE HempSize
hemp_hash_function_jenkins32(
    HempString      key,
    HempSize        length
);

void
hemp_hash_each(
    HempHash        hash,
    hemp_hash_iter  func
);

// for testing

HempText
hemp_hash_dump(
    HempHash        hash
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
