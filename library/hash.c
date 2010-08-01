#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "hemp/hash.h"
#include "hemp/debug.h"


static hemp_size_t primes[] = {
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

static hemp_size_t
hemp_hash_wider(
    hemp_size_t n
) {
    int i, max = sizeof(primes) / sizeof(primes[0]);
    for (i = 0; i < max; i++) {
        if (primes[i] > n)
            return primes[i];
    }
    return primes[max - 1];
}

hemp_hash_t 
hemp_hash_init() {
    hemp_hash_t table;
    hemp_size_t w;

    debug_mem("Allocating hash table\n");
    
    if ((table = (hemp_hash_t) hemp_mem_init(sizeof(struct hemp_hash)))) {
        debug_mem("Allocated hash table at %p\n", table);

        table->width   = hemp_hash_wider(0);
        table->hasher  = &hemp_hash_function;
//      table->hasher  = &hemp_hash_function_jenkins32;
        table->size    = 0;
        table->parent  = (hemp_hash_t) NULL;
        table->columns = (hemp_hash_entry_t *) hemp_mem_init(
            table->width * sizeof(hemp_hash_entry_t)
        );
        if (table->columns) {
            for(w = 0; w < table->width; w++) {
                table->columns[w] = (hemp_hash_entry_t) NULL;
            }
            debug_mem("Allocated hash at %p\n", table);
        }
        else {
            hemp_hash_null(table);
        }
    }
    return table;
}


void
hemp_hash_free(
    hemp_hash_t table
) {
    hemp_hash_entry_t entry, next;
    int i;

    debug_mem("Releasing hash at %p\n", table);

    if (table) {
        for(i = 0; i < table->width; i++) {
            entry = table->columns[i];
            while (entry) {
                next = entry->next;
                hemp_mem_free(entry);   /* what about what's in the entry? */
                entry = next;
            }
        }
        hemp_mem_free(table->columns);
        hemp_mem_free(table);
    }
}


hemp_size_t
hemp_hash_resize(
    hemp_hash_t table
) {
    hemp_size_t width, wider, hash, i;
    hemp_hash_entry_t *columns, entry, next;
    
    width = table->width;
    wider = hemp_hash_wider(width);

    if (width == wider)
        return width;  /* can't go any bigger */

    debug_mem("Resizing hash at %p from %d to %d\n", table, width, wider);

    columns = (hemp_hash_entry_t *) hemp_mem_init(wider * sizeof(hemp_hash_entry_t));
    
    for (i = 0; i < table->width; i++) {
        entry = table->columns[i];
        while (entry) {
            next = entry->next;
            hash = entry->hash % wider;
            entry->next = columns[i];
            columns[i] = entry;
            entry = next;
        }
    }
    hemp_mem_free(table->columns);
    table->columns = columns;
    table->width   = wider;
    return wider;
}


hemp_hash_entry_t
hemp_hash_store(hemp_hash_t table, hemp_cstr_t key, hemp_ptr_t value) {
    hemp_hash_entry_t entry;
    hemp_size_t hash, column;

    if (table->size / table->width > HEMP_HASH_DENSITY)
        hemp_hash_resize(table);

    hash   = (table->hasher)(key);
    column = hash % table->width;
    entry  = table->columns[column];

    while (entry && strcmp(key, entry->key)) {
        entry = entry->next;
    }

    if (entry)
        entry->value = value;
    else {
        entry  = hemp_mem_init(sizeof(struct hemp_hash_entry));    /* TODO: pool */
        entry->key   = key;
        entry->value = value;
        entry->hash  = hash;
        entry->next  = table->columns[column];
        table->columns[column] = entry;
        table->size++;
    }
    return entry;
}


hemp_ptr_t
hemp_hash_fetch(
    hemp_hash_t table, 
    hemp_cstr_t key
) {
    hemp_hash_entry_t entry = NULL;
    hemp_size_t hash, column;
    hash   = (table->hasher)(key);
    
    while (table && ! entry) {
        column = hash % table->width;
        entry  = table->columns[column];

        /* look for an entry in this hash table */
        while (entry && strcmp(key, entry->key))
            entry = entry->next;

        /* or try the parent table, if there is one */
        table = table->parent;
    }
    
    return entry 
        ? entry->value 
        : NULL;
}


void 
hemp_hash_set_parent(
    hemp_hash_t table, 
    hemp_hash_t parent
) {
    table->parent = parent;
}


hemp_hash_t
hemp_hash_get_parent(
    hemp_hash_t table
) {
    return table->parent;
}


void
hemp_hash_each(
    hemp_hash_t table, 
    void (* callback)(hemp_hash_entry_t)
) {
    hemp_size_t i;
    hemp_hash_entry_t entry;

    for (i = 0; i < table->width; i++) {
        entry = table->columns[i];
        while (entry) {
            (callback)(entry);
            entry = entry->next;
        }
    }
}


/*
hemp_cstr_t
hemp_hash_as_text(hemp_hash_t table) {
    hemp_size_t i, s = table->size;
    hemp_hash_entry_t entry;
    HEMP_VALUE  value;
    HEMP_TEXT   text = hemp_text_init(4 + table->size * 32);
    
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
hemp_hash_function(register hemp_cstr_t s) {
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
static hemp_size_t jenkins32_seed = 42;
typedef  unsigned long int  u4;   
typedef  unsigned     char  u1;

/* The mixing step */
#define jenkins32_mix(a,b,c) \
{ \
  a=a-b;  a=a-c;  a=a^(c>>13); \
  b=b-c;  b=b-a;  b=b^(a<<8);  \
  c=c-a;  c=c-b;  c=c^(b>>13); \
  a=a-b;  a=a-c;  a=a^(c>>12); \
  b=b-c;  b=b-a;  b=b^(a<<16); \
  c=c-a;  c=c-b;  c=c^(b>>5);  \
  a=a-b;  a=a-c;  a=a^(c>>3);  \
  b=b-c;  b=b-a;  b=b^(a<<10); \
  c=c-a;  c=c-b;  c=c^(b>>15); \
}

hemp_size_t
hemp_hash_function_jenkins32(register hemp_cstr_t key) {
   register u4 a,b,c;           /* the internal state */
   hemp_size_t length, remain;  /* total length, remaining left to process */

   /* Set up the internal state */
   length = remain = strlen(key);
   a = b = 0x9e3779b9;      /* the golden ratio; an arbitrary value */
   c = jenkins32_seed;      /* variable initialization of internal state */

   /* handle all but the last 11 bytes */
   while (remain >= 12)
   {
      a=a+(key[0]+((u4)key[1]<<8)+((u4)key[ 2]<<16)+((u4)key[ 3]<<24));
      b=b+(key[4]+((u4)key[5]<<8)+((u4)key[ 6]<<16)+((u4)key[ 7]<<24));
      c=c+(key[8]+((u4)key[9]<<8)+((u4)key[10]<<16)+((u4)key[11]<<24));
      jenkins32_mix(a,b,c);
      key=key+12; 
      remain=remain-12;
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
   jenkins32_mix(a,b,c);

   return c;
}
