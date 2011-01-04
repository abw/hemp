#include <hemp/type/hash.h>
#include <hemp/context.h>


static HempSize 
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

HEMP_TYPE(hemp_type_hash) {
    HempType type   = hemp_type_subtype(HempTypeValue, id, name);
    type->text      = &hemp_type_hash_text;
    type->fetch     = &hemp_type_hash_fetch;
    type->store     = &hemp_type_hash_store;
    type->dot       = &hemp_type_hash_dot;
    type->pairs     = &hemp_type_hash_pairs;
    type->boolean   = &hemp_value_true;            /* hash is always true or use hash size? */
    type->defined   = &hemp_value_true;

    hemp_type_extend(type, "length", &hemp_method_hash_length);

    return type;
};


/*--------------------------------------------------------------------------
 * General purpose hash functions
 *--------------------------------------------------------------------------*/

HEMP_DO_INLINE static HempSize
hemp_hash_wider(
    HempSize n
) {
    int i, max = sizeof(hemp_primes) / sizeof(hemp_primes[0]);
    for (i = 0; i < max; i++) {
        if (hemp_primes[i] > n)
            return hemp_primes[i];
    }
    return hemp_primes[max - 1];
}


HempHash
hemp_hash_init(
    HempHash hash
) {
    HEMP_INSTANCE(hash);
    HempSize w;

    /* hemp_debug_mem("Allocated hash table at %p\n", table); */

    hash->width  = hemp_hash_wider(0);
    hash->size   = 0;
    hash->parent = NULL;
    hash->slots  = (HempSlot *) hemp_mem_alloc(
        hash->width * sizeof(HempSlot)
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
    HempHash hash
) {
    HempSlot slot, next;
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
    HempHash hash
) {
    hemp_hash_release(hash);
    hemp_mem_free(hash);
}


HEMP_INLINE HempSize
hemp_hash_grow(
    HempHash hash
) {
    HempSize width, wider, index, i;
    HempSlot *slots, slot, next;
    
    width = hash->width;
    wider = hemp_hash_wider(width);

    if (width == wider)
        return width;   /* can't go any bigger */

    /* hemp_debug_mem("Resizing hash at %p from %d to %d\n", hash, width, wider); */

    slots = (HempSlot *) hemp_mem_alloc(
        wider * sizeof(HempSlot)
    );
    // TODO: macro this and test HEMP_HAVE_MEMSET
    memset(slots, 0, wider * sizeof(HempSlot));

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


HEMP_INLINE HempSlot
hemp_hash_store_keylen(
    HempHash   hash,
    HempString name,
    HempValue  value,
    HempSize   length
) {
    HempSize index  = hemp_hash_function(name, length);
    HempSize column;
    HempSlot slot;

    if (hash->size / hash->width > HEMP_HASH_DENSITY)
        hemp_hash_grow(hash);

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

//    hemp_debug("*** HASH STORE: %s\n", name);

    return slot;
}


HempSlot
hemp_hash_store(
    HempHash   hash,
    HempString name,
    HempValue  value
) {
    return hemp_hash_store_keylen(
        hash, name, value, strlen(name)
    );
}


HEMP_INLINE HempValue
hemp_hash_fetch_keylen(
    HempHash   hash,
    HempString name,
    HempSize   length
) {
    hemp_debug_call("hemp_hash_fetch_keylen(%p, %s, %d)\n", hash, name, length);
    HempSlot slot  = NULL;
    HempSize index = hemp_hash_function(name, length);
    HempSize column;
    
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


HempValue
hemp_hash_fetch_dotted(
    HempHash       hash,
    HempString     name,
    HempContext    context
) {
    /* if the name doesn't have a dot in it, then a simple fetch will do */
    if (! strchr(name, HEMP_DOT))
        return hemp_hash_fetch(hash, name);

    /* function to find a dotted value rooted in a hash, e.g. foo.bar.baz
     * which might be represented as foo => { bar => { baz => 10 } }
     */
    HempList       nodes  = hemp_string_split(name, HEMP_STR_DOT);
    HempValue      result = HempMissing;
    HempValue      node, value;
    HempSize       n;

    // TODO: this could be optimised, perhaps by pre-calculating the length
    // and hash value for each successively longer section of the path,
    // e.g. for foo.bar.baz, pre-compute foo, foo.bar and foo.bar.baz.
    while (hash) {                      /* walk up the parents */
        n     = 0;
        node  = hemp_list_item(nodes, n);
        value = hemp_hash_fetch(hash, hemp_val_str(node));

        while (hemp_is_found(value) && ++n < nodes->length) {
            node  = hemp_list_item(nodes, n);
            value = hemp_call(value, fetch, context, node);
        }

        if (hemp_is_found(value) && n == nodes->length) {
            result = value;
            break;
        }

        hash = hash->parent;
    }

    hemp_list_free(nodes);

    return result;
}


HempSlot
hemp_hash_store_dotted(
    HempHash       hash,
    HempString     name,
    HempValue      value,
    HempContext    context
) {
    hemp_debug_call("hemp_hash_store_dotted()\n");

    /* if the name doesn't have a dot in it, then a simple store will do */
    if (! strchr(name, HEMP_DOT))
        return hemp_hash_store(hash, name, value);

    HempList   nodes  = hemp_string_split(name, HEMP_STR_DOT);
    HempValue  parent = hemp_hash_val(hash);
    HempSize   max    = nodes->length - 1;
    HempValue  node, child;
    HempSize   n;

//  hemp_debug_msg("%d nodes in %s\n", nodes->length, name);

    for (n = 0; n < max; n++) {
        node  = hemp_list_item(nodes, n);
        child = hemp_fetch(parent, context, node);

//      HempString tmp_node = hemp_to_string(node, context);
//      hemp_debug_msg("[%d] %s => %s\n", n, tmp_node, hemp_type_name(child));

        if (hemp_is_undef(child)) {
//          hemp_debug_msg("adding auto-vivifed hash\n");
            HempHash autoviv = hemp_context_tmp_hash(context);
            child = hemp_hash_val(autoviv);
            hemp_store(parent, context, node, child);
        }
        parent = child;
    }

    node = hemp_list_item(nodes, max);
    hemp_store(parent, context, node, value);

    hemp_list_free(nodes);

    /* Ah. We don't always have a slot because we're not making the assumption
     * that the config always contains hemp hashes.  It might also contain
     * other hemp-like object which implement fetch/store methods.
     */
    return NULL;
}


HempValue
hemp_hash_delete(
    HempHash       hash,
    HempString     name
) {
    hemp_debug_call("hemp_hash_delete(%p, %s)\n", hash, name);
    HempSize   length = strlen(name);
    HempSize   index  = hemp_hash_function(name, length);
    HempSlot   slot   = NULL;
    HempSlot   prev   = NULL;
    HempValue  result = HempMissing;
    HempSize   column;
    
    column = index % hash->width;
    slot   = hash->slots[column];

    /* look for an entry in this hash table */
    while (slot 
      &&  (index != slot->index) 
      &&  ! hemp_hash_key_match(slot->name, name, length) ) {
        prev = slot;
        slot = slot->next;
    }

    if (slot) {
//      hemp_debug_msg("found slot at %p with key: %s\n", slot, slot->name);
        if (prev) {
//          hemp_debug_msg("previous slot found\n");
            prev->next = slot->next;
        }
        else {
//          hemp_debug_msg("slot is first in column\n");
            hash->slots[column] = slot->next;
        }
        result = slot->value;
        hemp_slot_free(slot);
    }

    return result;
}


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


void
hemp_hash_each(
    HempHash      hash,
    hemp_hash_iter func
) {
    HempSize   i;
    HempSlot   slot;
    HempPos    n = 0;

    for (i = 0; i < hash->width; i++) {
        slot = hash->slots[i];

        while (slot) {
            if (! func(hash, n++, slot))
                break;
            slot = slot->next;
        }
    }
}



/*---------------------------------------------------------------------
 * Default hash algorithm 
 * Can't remember where I got this from.  It's probably Perl's or 
 * maybe Python's?
 *---------------------------------------------------------------------*/

HempSize
hemp_hash_function_default(
    register HempString s
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
static HempSize hemp_hash_jenkins32_seed = 42;

#define hemp_keychar(k,i,s) \
    ((HempU32) k[i] << s)

#define hemp_keychars(k,a,b,c,d) \
    (k[a] + hemp_keychar(k,b,8) + hemp_keychar(k,c,16) + hemp_keychar(k,d,24))

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

HEMP_INLINE HempSize
hemp_hash_function_jenkins32(
    register HempString key,
    register HempSize   length
) {
    register HempU32    a, b, c;
    register HempSize   remain = length;

   /* Set up the internal state */
    a = b = 0x9e3779b9;
    c = hemp_hash_jenkins32_seed;

    /* handle all but the last 11 bytes */
    while (remain >= 12) {
        a = a + hemp_keychars(key, 0, 1, 2, 3);
        b = b + hemp_keychars(key, 4, 5, 6, 7);
        c = c + hemp_keychars(key, 8, 9, 10, 11);
        hemp_hash_jenkins32_mix(a,b,c);
        key    = key    + 12; 
        remain = remain - 12;
    }

    /* handle the last 11 bytes */
    c = c + length;
    switch(remain) {
        /* all the case statements fall through */
        case 11: c = c + hemp_keychar(key, 10, 24);
        case 10: c = c + hemp_keychar(key,  9, 16);
        case 9 : c = c + hemp_keychar(key,  8, 8);
        /* the first byte of c is reserved for the length */
        case 8 : b = b + hemp_keychar(key,  7, 24);
        case 7 : b = b + hemp_keychar(key,  6, 16);
        case 6 : b = b + hemp_keychar(key,  5,  8);
        case 5 : b = b + key[4];
        case 4 : a = a + hemp_keychar(key,  3, 24);
        case 3 : a = a + hemp_keychar(key,  2, 16);
        case 2 : a = a + hemp_keychar(key,  1,  8);
        case 1 : a = a + key[0];
        /* case 0: nothing left to add */
    }
    hemp_hash_jenkins32_mix(a,b,c);

    return c;
}


/*--------------------------------------------------------------------------
 * Runtime hash evaluation methods
 *--------------------------------------------------------------------------*/

HEMP_OUTPUT(hemp_type_hash_text) {
    HempText text;
    hemp_prepare_text(context, output, text);
    hemp_text_append_string(text, "TODO: hash.text");
    return output;
}


HEMP_FETCH(hemp_type_hash_fetch) {
    HempBool kmine  = HEMP_FALSE;
    HempText ktext;

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
    HempValue result = hemp_hash_fetch_keylen( 
        hemp_val_hash(container),
        ktext->string,
        ktext->length
    );

    /* release the text memory and return result */
    if (kmine)
        hemp_text_free(ktext);

    return result;
}


HEMP_STORE(hemp_type_hash_store) {
    hemp_debug_call("hemp_type_hash_store()\n");
    HempString keystr  = hemp_to_string(key, context);
    HempSlot   slot    = hemp_hash_store(
        hemp_val_hash(container),
        keystr,
        value
    );
    
    /* should we have a dedicated slot type? */
    return hemp_ptr_val(slot);
}


HEMP_FETCH(hemp_type_hash_dot) {
    HempBool kmine  = HEMP_FALSE;
    HempText ktext;

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
    HempValue result = hemp_hash_fetch_keylen( 
        hemp_val_hash(container),
        ktext->string,
        ktext->length
    );

    /* if we didn't find it then look for a method */
    if (hemp_is_missing(result)) {
        /* we'll do this manually because we already know the key length */
        /* result = hemp_send(container, ktext->string, context); */

        result = hemp_hash_fetch_keylen(
            hemp_vtype(container)->methods,
            ktext->string,
            ktext->length
        );
        
        if (hemp_is_found(result)) {
            HempEval method = (HempEval) hemp_val_ptr(result);
            result = method(container, context);
        }
    }

    /* release the text memory and return result */
    if (kmine)
        hemp_text_free(ktext);

    return result;
}


HEMP_OUTPUT(hemp_type_hash_pairs) {
    hemp_debug_call("hemp_type_hash_pairs()\n");

    HempHash   hash    = hemp_val_hash(value);
    HempHash   pairs;
    HempSlot   slot;
    HempSize   i;

    hemp_prepare_pairs(context, output, pairs);

    for (i = 0; i < hash->width; i++) {
        slot = hash->slots[i];
        while (slot) {
            hemp_hash_store(pairs, slot->name, slot->value);
            slot = slot->next;
        }
    }

    return output;
}


/*--------------------------------------------------------------------------
 * Hash pseudo-object methods
 *--------------------------------------------------------------------------*/

HEMP_VALUE(hemp_method_hash_length) {
    return hemp_int_val( hemp_val_hash(value)->size );
}



/*--------------------------------------------------------------------------
 * debugging methods
 *--------------------------------------------------------------------------*/

HempText hemp_hash_dump_buffer = NULL;
 
HempBool 
hemp_hash_dump_item(
    HempHash     hash,
    HempPos      index,
    HempSlot     item
) {
//  hemp_debug("dump hash item #%d: %s\n", index, item->name);
    hemp_text_append_string(hemp_hash_dump_buffer, "    ");
    hemp_text_append_string(hemp_hash_dump_buffer, item->name);
    hemp_text_append_string(hemp_hash_dump_buffer, " => ");
    hemp_text_append_string(hemp_hash_dump_buffer, hemp_type_name(item->value));
    hemp_text_append_string(hemp_hash_dump_buffer, ":");
    
    if (hemp_is_text(item->value)) {
        hemp_text_append_text(hemp_hash_dump_buffer, hemp_val_text(item->value));
    }
    else if (hemp_is_string(item->value)) {
        hemp_text_append_string(hemp_hash_dump_buffer, hemp_val_str(item->value));
    }
    else {
        hemp_text_append_string(hemp_hash_dump_buffer, hemp_type_name(item->value));
    }
    hemp_text_append_string(hemp_hash_dump_buffer, "\n");

    // ARSE!  We can't call the text method without a context
    // hemp_vtext(item->value, " => ");
    
    return HEMP_TRUE;
}


void 
hemp_hash_dump_hash(
    HempHash hash
) {
    char buffer[1000];
    sprintf(buffer, "[%p] ", hash);
//  hemp_debug("dump hash: %p (%d items)\n", hash, hash->size);
    hemp_text_append_string(hemp_hash_dump_buffer, buffer);
    hemp_text_append_string(hemp_hash_dump_buffer, "{\n");
    hemp_hash_each(hash, &hemp_hash_dump_item);
    hemp_text_append_string(hemp_hash_dump_buffer, "}\n");
    if (hash->parent) {
        hemp_text_append_string(hemp_hash_dump_buffer, "\nPARENT => ");
        hemp_hash_dump_hash(hash->parent);
    }
}

HempText
hemp_hash_dump(
    HempHash hash
) {
    hemp_hash_dump_buffer = hemp_text_new();
    hemp_hash_dump_hash(hash);
    return hemp_hash_dump_buffer;
}

