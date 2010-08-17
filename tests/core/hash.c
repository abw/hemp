#include <hemp/test.h>

void test_hash();
void hash_get(hemp_hash_p, char *);
void hash_set(hemp_hash_p table, hemp_cstr_p, hemp_cstr_p);
void hash_has(hemp_hash_p table, hemp_cstr_p, hemp_cstr_p);
void hash_hasnt(hemp_hash_p table, hemp_cstr_p);


int main(
    int argc, char **argv, char **env
) {
    plan(10);
    test_hash();
    return done();
}


void test_hash() {
    hemp_hash_p table = hemp_hash_init();
    hemp_hash_p child = hemp_hash_init();

    hash_set(table, "msg", "hello world");
    hash_has(table, "msg", "hello world");

    hash_set(child, "foo", "The foo item");
    hash_has(child, "foo", "The foo item");

    hash_hasnt(child, "msg");
    
    hemp_hash_attach(child, table);
    
    hash_has(child, "foo", "The foo item");
    hash_has(child, "msg", "hello world");    

    hash_set(table, "bar", "The bar item");
    hash_has(child, "bar", "The bar item");    

    hemp_hash_detach(child);

    hash_hasnt(child, "msg");

    hemp_hash_free(table);
    hemp_hash_free(child);
}


void 
hash_has(
    hemp_hash_p table, 
    hemp_cstr_p key, 
    hemp_cstr_p expect
) {
    hemp_mem_p value = hemp_hash_fetch(table, key);
    ok( 
        value && hemp_cstr_eq(value, expect),
        "found %s => %s", key, value
    );
}


void 
hash_hasnt(
    hemp_hash_p table, 
    hemp_cstr_p key
) {
    hemp_mem_p value = hemp_hash_fetch(table, key);
    ok( ! value, "no entry for %s", key);
}


void 
hash_set(
    hemp_hash_p table, 
    hemp_cstr_p key, 
    hemp_cstr_p value
) {
    ok(
        hemp_hash_store(table, key, value),
        "set %s to %s", key, value
    );
}
