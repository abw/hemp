#include <stdio.h>
#include "hemp.h"
#include "tap.h"

void test_hash();
void hash_get(hemp_hash_t, char *);
void hash_set(hemp_hash_t table, char *key, char *value);
void hash_has(hemp_hash_t table, char *key, char *expect);
void hash_hasnt(hemp_hash_t table, char *key);


int
main(int argc, char **argv, char **env)
{
    plan_tests(9);
    test_hash();
    return exit_status();
}


void test_hash() {
    hemp_hash_t table = hemp_hash_init();
    hemp_hash_t child = hemp_hash_init();

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

    hemp_hash_free(table);
    hemp_hash_free(child);
}


void 
hash_has(hemp_hash_t table, char *key, char *expect) 
{
    hemp_ptr_t value = hemp_hash_fetch(table, key);
    ok( 
        value && hemp_str_eq(value, expect),
        "found %s => %s", key, value
    );
}


void 
hash_hasnt(hemp_hash_t table, char *key) 
{
    hemp_ptr_t value = hemp_hash_fetch(table, key);
    ok( ! value, "no entry for %s", key);
}


void 
hash_set(hemp_hash_t table, char *key, char *value) 
{
    ok(
        hemp_hash_store(table, key, value),
        "set %s to %s", key, value
    );
}
