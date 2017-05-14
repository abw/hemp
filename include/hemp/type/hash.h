#ifndef HEMP_HASH_H
#define HEMP_HASH_H

#include <hemp/value.h>
#include <hemp/slot.h>


/*--------------------------------------------------------------------------
 * Type definitions
 *--------------------------------------------------------------------------*/

struct hemp_hash {
    HempSize        width;      /* number of columns        */
    HempSize        size;       /* total number of entries  */
    HempSlot      * slots;      /* heads of slot columns    */
    HempHash        parent;     /* parent hash              */
};


/*--------------------------------------------------------------------------
 * Function prototypes
 *--------------------------------------------------------------------------*/

HempHash
hemp_hash_init();

void
hemp_hash_release(
    HempHash        hash
);

void
hemp_hash_free(
    HempHash        hash
);

HempSize
hemp_hash_grow(
    HempHash        hash
);

HempSlot
hemp_hash_store_keylen(
    HempHash        hash,
    HempString      name,
    HempValue       value,
    HempSize        length
);

HempValue
hemp_hash_fetch_keylen(
    HempHash        hash,
    HempString      name,
    HempSize        length
);

HempValue
hemp_hash_fetch_dotted(
    HempHash        hash,
    HempString      name,
    HempContext     context
);

HempSlot
hemp_hash_store_dotted(
    HempHash        hash,
    HempString      name,
    HempValue       value,
    HempContext     context
);

HempValue
hemp_hash_delete(
    HempHash        hash,
    HempString      name
);

void
hemp_hash_each(
    HempHash        hash,
    hemp_hash_iter  func
);

//HempString
//hemp_hash_as_string(
//    HempHash        hash
//);

HempSize
hemp_hash_function_default(
    HempString      key
);

HempSize
hemp_hash_function_jenkins32(
    HempString      key,
    HempSize        length
);

// for testing
HempText
hemp_hash_dump(
    HempHash        hash
);


/*--------------------------------------------------------------------------
 * Inline functions
 *--------------------------------------------------------------------------*/

HEMP_INLINE void
hemp_hash_attach(
    HempHash child,
    HempHash parent
) {
    child->parent = parent;
}


HEMP_INLINE void
hemp_hash_detach(
    HempHash child
) {
    child->parent = NULL;
}

HEMP_INLINE HempSlot
hemp_hash_store(
    HempHash   hash,
    HempString name,
    HempValue  value
) {
    return hemp_hash_store_keylen(
        hash, name, value, strlen(name)
    );
}


HEMP_INLINE HempBool
hemp_hash_key_match(
    HempString key1,
    HempString key2,
    HempSize   length
) {
    /* We allow hash keys to be looked up using an unterminated C string
     * (with length specified explicitly) so that we can use an element's
     * source as the lookup key (e.g. in the document "Hello [% name %]"
     * we can pass a pointer to the 9th character and length 4 to lookup
     * the value for "name" without having to allocate and prepare a short
     * C string to hold the hash key for lookup.  One side effect of this
     * is that we can't use the normal strcmp() or strncmp() functions, so
     * we roll a simple one of our own which matches all of the hash key
     * against the first 'length' characters of the search key.  In other
     * words, the search key may be longer, but the slot key can't be
     */
    while (*key1 && *key1 == *key2 && length--) {
        key1++;
        key2++;
    }

//  hemp_debug("\nlength: %d  k1: [%c]  k2: [%c]\n", length, *key1, *key2);

    return (length || *key1)
        ? HEMP_FALSE
        : HEMP_TRUE;
}

HEMP_INLINE HempValue
hemp_hash_fetch(
    HempHash   hash,
    HempString name
) {
    return hemp_hash_fetch_keylen(
        hash, name, strlen(name)
    );
}

HEMP_INLINE HempNum
hemp_hash_fetch_number(
    HempHash   hash,
    HempString name
) {
    HempValue value = hemp_hash_fetch(hash, name);

    /* this is of limited value because we have no way to indicate "not
     * found" when using raw numeric values (NaN perhaps?)
     */
    return hemp_is_number(value)
        ? hemp_val_num(value)
        : 0.0;
}

HEMP_INLINE HempInt
hemp_hash_fetch_integer(
    HempHash   hash,
    HempString name
) {
    HempValue value = hemp_hash_fetch(hash, name);

    /* as above, only for testing, or when you know all your values are
     * non-zero
     */
    return hemp_is_integer(value)
        ? hemp_val_int(value)
        : 0;
}

HEMP_INLINE HempMemory
hemp_hash_fetch_pointer(
    HempHash   hash,
    HempString name
) {
    HempValue value = hemp_hash_fetch(hash, name);

    return hemp_is_pointer(value)
        ? hemp_val_ptr(value)
        : NULL;
}

HEMP_INLINE HempString
hemp_hash_fetch_string(
    HempHash   hash,
    HempString name
) {
    HempValue value = hemp_hash_fetch(hash, name);

    return hemp_is_string(value)
        ? hemp_val_str(value)
        : NULL;
}

HEMP_INLINE HempText
hemp_hash_fetch_text(
    HempHash   hash,
    HempString name
) {
    HempValue value = hemp_hash_fetch(hash, name);

    return hemp_is_text(value)
        ? hemp_val_text(value)
        : NULL;
}

HEMP_INLINE HempList
hemp_hash_fetch_list(
    HempHash   hash,
    HempString name
) {
    HempValue value = hemp_hash_fetch(hash, name);

    return hemp_is_list(value)
        ? hemp_val_list(value)
        : NULL;
}

HEMP_INLINE HempHash
hemp_hash_fetch_hash(
    HempHash   hash,
    HempString name
) {
    HempValue value = hemp_hash_fetch(hash, name);

    return hemp_is_hash(value)
        ? hemp_val_hash(value)
        : NULL;
}


/*--------------------------------------------------------------------------
 * Type functions and methods
 *--------------------------------------------------------------------------*/

HEMP_TYPE(hemp_type_hash);
HEMP_OUTPUT(hemp_type_hash_text);
HEMP_FETCH(hemp_type_hash_fetch);
HEMP_STORE(hemp_type_hash_store);
HEMP_FETCH(hemp_type_hash_dot);
HEMP_OUTPUT(hemp_type_hash_pairs);
HEMP_VALUE(hemp_method_hash_length);


/*--------------------------------------------------------------------------
 * Patch in the hash function we want to use and define some other macros
 *--------------------------------------------------------------------------*/

#define hemp_hash_function(string, len)         \
    hemp_hash_function_jenkins32(string, len)

#define hemp_hash_store_number(h, k, n)         \
    hemp_hash_store(h, k, hemp_num_val(n))

#define hemp_hash_store_integer(h, k, i)        \
    hemp_hash_store(h, k, hemp_int_val(i))

#define hemp_hash_store_pointer(h, k, p)        \
    hemp_hash_store(h, k, hemp_ptr_val(p))

#define hemp_hash_store_string(h, k, s)         \
    hemp_hash_store(h, k, hemp_str_val(s))

#define hemp_hash_store_text(h, k, t)           \
    hemp_hash_store(h, k, hemp_text_val(t))

#define hemp_hash_store_list(h, k, l)           \
    hemp_hash_store(h, k, hemp_list_val(l))

#define hemp_hash_store_hash(h, k, v)           \
    hemp_hash_store(h, k, hemp_hash_val(v))

#define hemp_hash_new()                         \
    hemp_hash_init(NULL)


#endif /* HEMP_HASH_H */
