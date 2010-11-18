#include <hemp/type/hash.h>
#include <hemp/context.h>


static hemp_size_t 
hemp_primes[] = {
    8 + 3,
    16 + 3,
    32 + 5,
    64 + 3,
    128 + 3,
    256 + 27,
    512 + 9,
    1024 + 9,
    2048 + 5,
    4096 + 3,
    8192 + 27,
    16384 + 43,
    32768 + 3,
    65536 + 45,
    131072 + 29,
    262144 + 3,
    524288 + 21,
    1048576 + 7,
    2097152 + 17,
    4194304 + 15,
    8388608 + 9,
    16777216 + 43,
    33554432 + 35,
    67108864 + 15,
    134217728 + 29,
    268435456 + 3,
    536870912 + 11,
    1073741824 + 85,
};


/*--------------------------------------------------------------------------
 * Type constructor
 *--------------------------------------------------------------------------*/

HEMP_TYPE_FUNC(hemp_type_hash) {
    hemp_type_p type = hemp_type_subtype(HempValue, id, name);
    type->text       = &hemp_type_hash_text;
    type->fetch      = &hemp_type_hash_fetch;
    type->dot        = &hemp_type_hash_dot;
    type->boolean    = &hemp_value_true;            /* hash is always true or use hash size? */
    type->defined    = &hemp_value_true;

    hemp_type_extend(type, "length", &hemp_method_hash_length);

    return type;
};


/*--------------------------------------------------------------------------
 * General purpose hash functions
 *--------------------------------------------------------------------------*/

HEMP_DO_INLINE static hemp_size_t
hemp_hash_wider(
    hemp_size_t n
) {
    int i, max = sizeof(hemp_primes) / sizeof(hemp_primes[0]);
    for (i = 0; i < max; i++) {
        if (hemp_primes[i] > n)
            return hemp_primes[i];
    }
    return hemp_primes[max - 1];
}


hemp_hash_p
hemp_hash_init() {
    hemp_size_t w;
    hemp_hash_p hash = (hemp_hash_p) hemp_mem_alloc(
        sizeof(struct hemp_hash_s)
    );
    
    if (! hash)
        hemp_mem_fail("hash");

    /* hemp_debug_mem("Allocated hash table at %p\n", table); */

    hash->width  = hemp_hash_wider(0);
    hash->size   = 0;
    hash->parent = NULL;
    hash->slots  = (hemp_slot_p *) hemp_mem_alloc(
        hash->width * sizeof(hemp_slot_p)
    );
    if (! hash->slots)
        hemp_mem_fail("hash slots");

    // TODO: hemp_mem_wipe(ptr,size) or perhaps hemp_mem_init(size) to 
    // allocate memory, alloc+ wie

    /* hemp_debug_mem("Allocated hash columns at %p\n", table->columns); */
    for(w = 0; w < hash->width; w++) {
        hash->slots[w] = NULL;
    }

    return hash;
}


HEMP_INLINE void
hemp_hash_release(
    hemp_hash_p hash
) {
    hemp_slot_p slot, next;
    int i;

//  hemp_debug_mem("Releasing hash at %p\n", hash);

    for(i = 0; i < hash->width; i++) {
        slot = hash->slots[i];

        while (slot) {
            next = slot->next;
            hemp_slot_free(slot);
            slot = next;
        }
    }
    hemp_mem_free(hash->slots);
}


void
hemp_hash_free(
    hemp_hash_p hash
) {
    hemp_hash_release(hash);
    hemp_mem_free(hash);
}


HEMP_INLINE hemp_size_t
hemp_hash_resize(
    hemp_hash_p hash
) {
    hemp_size_t width, wider, index, i;
    hemp_slot_p *slots, slot, next;
    
    width = hash->width;
    wider = hemp_hash_wider(width);

    if (width == wider)
        return width;   /* can't go any bigger */

    /* hemp_debug_mem("Resizing hash at %p from %d to %d\n", hash, width, wider); */

    slots = (hemp_slot_p *) hemp_mem_alloc(
        wider * sizeof(hemp_slot_p)
    );

    for (i = 0; i < hash->width; i++) {
        slot = hash->slots[i];
        while (slot) {
            next         = slot->next;
            index        = slot->index % wider;
            slot->next   = slots[index];
            slots[index] = slot;
            slot         = next;
        }
    }

    hemp_mem_free(hash->slots);
    hash->slots = slots;
    hash->width = wider;

    return wider;
}


HEMP_INLINE hemp_bool_t 
hemp_hash_key_match(
    hemp_str_p  key1,
    hemp_str_p  key2,
    hemp_size_t length
) {
    /* We allow hash keys to be looked up using an unterminated C string
     * (with length specified explicitly) so that we can use an element's
     * source as the lookup key (e.g. in the template "Hello [% name %]"
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


HEMP_INLINE hemp_slot_p
hemp_hash_store_keylen(
    hemp_hash_p  hash,
    hemp_str_p   name,
    hemp_value_t value,
    hemp_size_t  length
) {
    hemp_size_t index  = hemp_hash_function(name, length);
    hemp_size_t column;
    hemp_slot_p slot;

    if (hash->size / hash->width > HEMP_HASH_DENSITY)
        hemp_hash_resize(hash);

    column = index % hash->width;
    slot   = hash->slots[column];

    /* look at each entry in the column comparing the numerical hash value 
     * first, and then the more time consuming key string comparison only if 
     * the hash values are identical
     */
    while (slot 
      &&  (index != slot->index) 
      &&  ! hemp_hash_key_match(slot->name, name, length) )
        slot = slot->next;

    if (slot) {
        /* TODO: free the value in the slot */
        slot->value = value;
    }
    else {
        slot = hash->slots[column] = hemp_slot_init(
            hemp_ptr_val(hash), index, name, value, hash->slots[column]
        );
        hash->size++;
    }

    return slot;
}


hemp_slot_p
hemp_hash_store(
    hemp_hash_p  hash,
    hemp_str_p   name,
    hemp_value_t value
) {
    return hemp_hash_store_keylen(
        hash, name, value, strlen(name)
    );
}


HEMP_INLINE hemp_value_t
hemp_hash_fetch_keylen(
    hemp_hash_p hash, 
    hemp_str_p  name,
    hemp_size_t length
) {
    hemp_debug_call("hemp_hash_fetch_keylen(%p, %s, %d)\n", hash, name, length);
    hemp_slot_p slot  = NULL;
    hemp_size_t index = hemp_hash_function(name, length);
    hemp_size_t column;
    
//  hemp_debug("looking up slot [%s][%d] => %ud\n", name, length, index);
    
    while (hash && ! slot) {
        column = index % hash->width;
        slot   = hash->slots[column];

        /* look for an entry in this hash table */
        while (slot 
          &&  (index != slot->index) 
          &&  ! hemp_hash_key_match(slot->name, name, length) )
            slot = slot->next;

        /* or try the parent table, if there is one */
        hash = hash->parent;
    }

//    if (slot) {
//        hemp_debug("found slot at %p with key: %s\n", slot, slot->name);
//    }
//    else {
//        hemp_debug("no slot\n");
//    }

    return slot
        ? slot->value 
        : HempMissing;
}


HEMP_INLINE hemp_value_t
hemp_hash_fetch(
    hemp_hash_p hash, 
    hemp_str_p  name
) {
    return hemp_hash_fetch_keylen(
        hash, name, strlen(name)
    );
}


hemp_num_t
hemp_hash_fetch_number(
    hemp_hash_p hash, 
    hemp_str_p  name
) {
    hemp_value_t value = hemp_hash_fetch(hash, name);

    /* this is of limited value because we have no way to indicate "not 
     * found" when using raw numeric values (NaN perhaps?)
     */
    return hemp_is_number(value)
        ? hemp_val_num(value)
        : 0.0;
}


hemp_int_t
hemp_hash_fetch_integer(
    hemp_hash_p hash, 
    hemp_str_p  name
) {
    hemp_value_t value = hemp_hash_fetch(hash, name);

    /* as above, only for testing, or when you know all your values are 
     * non-zero 
     */
    return hemp_is_integer(value)
        ? hemp_val_int(value)
        : 0;
}


hemp_mem_p
hemp_hash_fetch_pointer(
    hemp_hash_p hash, 
    hemp_str_p  name
) {
    hemp_value_t value = hemp_hash_fetch(hash, name);

    return hemp_is_pointer(value)
        ? hemp_val_ptr(value)
        : NULL;
}


hemp_str_p
hemp_hash_fetch_string(
    hemp_hash_p hash, 
    hemp_str_p  name
) {
    hemp_value_t value = hemp_hash_fetch(hash, name);

    return hemp_is_string(value)
        ? hemp_val_str(value)
        : NULL;
}


hemp_text_p
hemp_hash_fetch_text(
    hemp_hash_p hash, 
    hemp_str_p  name
) {
    hemp_value_t value = hemp_hash_fetch(hash, name);

    return hemp_is_text(value)
        ? hemp_val_text(value)
        : NULL;
}



HEMP_INLINE void 
hemp_hash_attach(
    hemp_hash_p child, 
    hemp_hash_p parent
) {
    child->parent = parent;
}


HEMP_INLINE void 
hemp_hash_detach(
    hemp_hash_p child
) {
    child->parent = NULL;
}


void
hemp_hash_each(
    hemp_hash_p      hash,
    hemp_hash_each_f func
) {
    hemp_size_t i;
    hemp_slot_p slot;
    hemp_pos_t n = 0;


    for (i = 0; i < hash->width; i++) {
        slot = hash->slots[i];

        while (slot) {
            if (! func(hash, n++, slot))
                break;
            slot = slot->next;
        }
    }
}


/*
hemp_string_t
hemp_hash_as_text(hemp_hash_t table) {
    hemp_size_t i, s = table->size;
    hemp_hash_entry_t entry;
    HEMP_VALUE  value;
    HEMP_TEXT   text = hemp_text_new_size(4 + table->size * 32);
    
    hemp_text_append_string(text, "{ ");

    for (i = 0; i < table->width; i++) {
        entry = table->columns[i];
        while (entry) {
            value = entry->value;
            hemp_text_append_string(text, entry->key);
            hemp_text_append_string(text, ":");
            hemp_text_append_text(text, (HEMP_TEXT) value->type->text(value));
            entry = entry->next;
            if (--s)
                hemp_text_append_string(text, ", ");
        }
    }

    hemp_text_append_string(text, " }");
    return text;
}

void 
hemp_hash_entry_print(hemp_hash_entry_t entry) {
    printf("%s => %s\n", entry->key, HEMP_AS_STRING(entry->value));
}
    
void 
hemp_hash_print(hemp_hash_t table) {
    hemp_hash_each(table, &hemp_hash_entry_print);
}

*/


/*---------------------------------------------------------------------
 * Default hash algorithm 
 * Can't remember where I got this from.  It's probably Perl's or 
 * maybe Python's?
 *---------------------------------------------------------------------*/

hemp_size_t
hemp_hash_function_default(
    register hemp_str_p s
) {
    register int c;
    register unsigned int val = 0;

    while ((c = *s++) != '\0') {
        val += c;
        val += (val << 10);
        val ^= (val >> 6);
    }
    val += (val << 3);
    val ^= (val >> 11);

    return val + (val << 15);
}


/*---------------------------------------------------------------------
 * Jenkins32 Hash algorithm 
 * See http://burtleburtle.net/bob/hash/evahash.html
 *---------------------------------------------------------------------*/

/* Define an initial seed value and some type aliases */
static hemp_size_t hemp_hash_jenkins32_seed = 42;
typedef  unsigned long int  u4;   
typedef  unsigned     char  u1;

/* The mixing step */
#define hemp_hash_jenkins32_mix(a,b,c) {    \
    a=a-b;  a=a-c;  a=a^(c>>13);            \
    b=b-c;  b=b-a;  b=b^(a<<8);             \
    c=c-a;  c=c-b;  c=c^(b>>13);            \
    a=a-b;  a=a-c;  a=a^(c>>12);            \
    b=b-c;  b=b-a;  b=b^(a<<16);            \
    c=c-a;  c=c-b;  c=c^(b>>5);             \
    a=a-b;  a=a-c;  a=a^(c>>3);             \
    b=b-c;  b=b-a;  b=b^(a<<10);            \
    c=c-a;  c=c-b;  c=c^(b>>15);            \
}

HEMP_INLINE hemp_size_t
hemp_hash_function_jenkins32(
    register hemp_str_p  key,
    register hemp_size_t length
) {
    register u4 a, b, c;
    register hemp_size_t remain = length;

   /* Set up the internal state */
    a = b = 0x9e3779b9;
    c = hemp_hash_jenkins32_seed;

    /* handle all but the last 11 bytes */
    while (remain >= 12) {
        a = a+(key[0]+((u4)key[1]<<8)+((u4)key[ 2]<<16)+((u4)key[ 3]<<24));
        b = b+(key[4]+((u4)key[5]<<8)+((u4)key[ 6]<<16)+((u4)key[ 7]<<24));
        c = c+(key[8]+((u4)key[9]<<8)+((u4)key[10]<<16)+((u4)key[11]<<24));
        hemp_hash_jenkins32_mix(a,b,c);
        key    = key    + 12; 
        remain = remain - 12;
    }

    /* handle the last 11 bytes */
    c = c + length;
    switch(remain) {
        /* all the case statements fall through */
        case 11: c=c+((u4)key[10]<<24);
        case 10: c=c+((u4)key[9]<<16);
        case 9 : c=c+((u4)key[8]<<8);
        /* the first byte of c is reserved for the length */
        case 8 : b=b+((u4)key[7]<<24);
        case 7 : b=b+((u4)key[6]<<16);
        case 6 : b=b+((u4)key[5]<<8);
        case 5 : b=b+key[4];
        case 4 : a=a+((u4)key[3]<<24);
        case 3 : a=a+((u4)key[2]<<16);
        case 2 : a=a+((u4)key[1]<<8);
        case 1 : a=a+key[0];
        /* case 0: nothing left to add */
    }
    hemp_hash_jenkins32_mix(a,b,c);

    return c;
}


/*--------------------------------------------------------------------------
 * Runtime hash evaluation methods
 *--------------------------------------------------------------------------*/

HEMP_OUTPUT_FUNC(hemp_type_hash_text) {
    hemp_text_p text;
    hemp_prepare_text(context, output, text);
    hemp_text_append_string(text, "TODO: hash.text");
    return output;
}


HEMP_FETCH_FUNC(hemp_type_hash_fetch) {
    hemp_bool_t kmine  = HEMP_FALSE;
    hemp_text_p ktext;

    if (hemp_is_text(key)) {
        ktext = hemp_val_text(key);
    }
    else {
        /* otherwise we have to convert the key to text */
        ktext = hemp_text_new_size(16);
        kmine = HEMP_TRUE;
        hemp_onto_text(key, context, hemp_text_val(ktext));
    }

    hemp_debug_call(
        "hemp_type_hash_fetch(%s, %s)\n", 
        hemp_type_name(container), 
        ktext->string
    );
    /* fetch the value */
    hemp_value_t result = hemp_hash_fetch_keylen( 
        hemp_val_hash(container),
        ktext->string,
        ktext->length
    );

    /* release the text memory and return result */
    if (kmine)
        hemp_text_free(ktext);

    return result;
}


HEMP_FETCH_FUNC(hemp_type_hash_dot) {
    hemp_bool_t kmine  = HEMP_FALSE;
    hemp_text_p ktext;

    if (hemp_is_text(key)) {
        ktext = hemp_val_text(key);
    }
    else {
        /* otherwise we have to convert the key to text */
        ktext = hemp_text_new_size(16);
        kmine = HEMP_TRUE;
        hemp_onto_text(key, context, hemp_text_val(ktext));
    }

    hemp_debug_call(
        "hemp_hash_dot(%s, %s)\n", 
        hemp_type_name(container), 
        ktext->string
    );

    /* fetch the value */
    hemp_value_t result = hemp_hash_fetch_keylen( 
        hemp_val_hash(container),
        ktext->string,
        ktext->length
    );

    /* if we didn't find it then look for a method */
    if (hemp_is_missing(result)) {
        /* we'll do this manually because we already know the key length */
        /* result = hemp_send(container, ktext->string, context); */

        result = hemp_hash_fetch_keylen(
            hemp_type(container)->methods,
            ktext->string,
            ktext->length
        );
        
        if (hemp_is_found(result)) {
            hemp_value_f method = (hemp_value_f) hemp_val_ptr(result);
            result = method(container, context);
        }
    }

    /* release the text memory and return result */
    if (kmine)
        hemp_text_free(ktext);

    return result;
}



/*--------------------------------------------------------------------------
 * Hash pseudo-object methods
 *--------------------------------------------------------------------------*/

HEMP_VALUE_FUNC(hemp_method_hash_length) {
    return hemp_int_val( hemp_val_hash(value)->size );
}




