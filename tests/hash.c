#include <stdio.h>
#include "hemp.h"

void test_hash();
void hash_get(hemp_hash_t, char *);
void hash_set(hemp_hash_t table, char *key, char *value);
void hash_has(hemp_hash_t table, char *key, char *expect);
void hash_hasnt(hemp_hash_t table, char *key);


int
main(int argc, char **argv, char **env)
{
    test_hash();
    return 0;
}


void test_hash() {
    hemp_hash_t table = hemp_hash_init();
    hemp_hash_t child = hemp_hash_init();

    debug("Made new tables\n");
    
//    for (i = 0; i < 20; i++) {
//        hash_table_resize(table);
//        printf("width %d : %u\n", i, hash_width(i));
//    }
    debug("\nHASH TESTS\n----------\n");
    hash_set(table, "msg", "hello world");
    hash_has(table, "msg", "hello world");

    hash_set(child, "foo", "The foo item");
    hash_has(child, "foo", "The foo item");

    hash_hasnt(child, "msg");
    
    child->parent = table;
    
    hash_has(child, "foo", "The foo item");
    hash_has(child, "msg", "hello world");    

    hash_set(table, "bar", "The bar item");
    hash_has(child, "bar", "The bar item");    

//    hemp_hash_print(table);

    hemp_hash_free(table);
    hemp_hash_free(child);
}


void 
hash_has(hemp_hash_t table, char *key, char *expect) 
{
    hemp_ptr_t value = hemp_hash_fetch(table, key);
    if (value && hemp_str_eq(value, expect)) {
        pass("%s => %s", key, value);
    }
    else {
        fail("Not found: %s (expected %s)", key, expect);
    }
}


void 
hash_hasnt(hemp_hash_t table, char *key) 
{
    hemp_ptr_t value = hemp_hash_fetch(table, key);
    if (value) {
        fail("Found unexpected %s => %s", key, value);
    }
    else {
        pass("%s => NOT FOUND (as expected)", key);
    }
}


void 
hash_get(hemp_hash_t table, char *key) 
{
    hemp_ptr_t value = hemp_hash_fetch(table, key);
    if (value) {
        debug("%s => %s\n", key, value);
    }
    else {
        debug("Not found: %s\n", key);
    }
}

void 
hash_set(hemp_hash_t table, char *key, char *value) 
{
//    HEMP_TEXT text = hemp_text_from_string(value);
    hemp_hash_store(table, key, value);
}
