#include <hemp/test.h>

void test_hash();
void hash_get(hemp_hash_p, char *);
void hash_set(hemp_hash_p table, hemp_str_p, hemp_str_p);
void hash_has(hemp_hash_p table, hemp_str_p, hemp_str_p);
void hash_hasnt(hemp_hash_p table, hemp_str_p);
void hash_has_keylen(hemp_hash_p table, hemp_str_p, hemp_size_t, hemp_str_p);
void hash_hasnt_keylen(hemp_hash_p table, hemp_str_p, hemp_size_t);
void hash_index_same(hemp_str_p, hemp_size_t, hemp_str_p, hemp_size_t);
void hash_index_different(hemp_str_p, hemp_size_t, hemp_str_p, hemp_size_t);
void hash_keys_do_match(hemp_str_p, hemp_str_p, hemp_size_t);
void hash_keys_no_match(hemp_str_p, hemp_str_p, hemp_size_t);


int main(
    int argc, char **argv, char **env
) {
    plan(23);
    test_hash();
    return done();
}


void test_hash() {
    hemp_hash_p table = hemp_hash_init();
    hemp_hash_p child = hemp_hash_init();
    

    hash_index_same(
        "foo",  3,
        "food", 3
    );
    hash_index_different(
        "foo",  3,
        "food", 4
    );
    hash_index_same(
        "Rachmaninov",  6,
        "Rachmaninoff", 6
    );
    hash_index_same(
        "Rachmaninov",  10,
        "Rachmaninoff", 10
    );
    hash_index_different(
        "Rachmaninov",  11,
        "Rachmaninoff", 11
    );
    hash_index_different(
        "Rachmaninoff", 11,
        "Rachmaninoff", 12
    );

    /* it's OK for a key in the hash (first arg) to match a prefix substring
     * of the search key (second arg), but not OK for a search key to match a
     * prefix of the hash key
     */
    hash_keys_do_match("foo", "food", 3);
    hash_keys_no_match("food", "foo", 3);

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

    hash_has_keylen(child, "food", 3, "The foo item");
    hash_has_keylen(child, "msgism", 3, "hello world");    
    hash_hasnt_keylen(child, "msgism", 4);    

    hemp_hash_detach(child);

    hash_hasnt(child, "msg");
    hash_has_keylen(child, "food", 3, "The foo item");
    hash_hasnt_keylen(child, "msgism", 3);    

    hemp_hash_free(table);
    hemp_hash_free(child);
}


void hash_keys_do_match(
    hemp_str_p  hash_key,
    hemp_str_p  search_key,
    hemp_size_t length
) {
    ok(
        hemp_hash_key_match(hash_key, search_key, length),
        "match hash key [%s] with search key [%s] length [%d]", 
        hash_key, search_key, length
    );
}


void hash_keys_no_match(
    hemp_str_p  hash_key,
    hemp_str_p  search_key,
    hemp_size_t length
) {
    ok(
        ! hemp_hash_key_match(hash_key, search_key, length),
        "no hash key [%s] with search key [%s] length [%d]", 
        hash_key, search_key, length
    );
}

void hash_index_same(
    hemp_str_p  key1,
    hemp_size_t len1,
    hemp_str_p  key2,
    hemp_size_t len2
) {
    hemp_size_t hash1 = hemp_hash_function(key1, len1);
    hemp_size_t hash2 = hemp_hash_function(key2, len2);
    ok( hash1 == hash2, "hash(%s[%d]) == hash(%s[%d])", key1, len1, key2, len2); 
}


void hash_index_different(
    hemp_str_p  key1,
    hemp_size_t len1,
    hemp_str_p  key2,
    hemp_size_t len2
) {
    hemp_size_t hash1 = hemp_hash_function(key1, len1);
    hemp_size_t hash2 = hemp_hash_function(key2, len2);
    ok( hash1 != hash2, "hash(%s[%d]) != hash(%s[%d])", key1, len1, key2, len2); 
}


void 
hash_has(
    hemp_hash_p table, 
    hemp_str_p  key, 
    hemp_str_p  expect
) {
    hemp_str_p value = hemp_hash_fetch_string(table, key);
    ok( 
        value && hemp_string_eq(value, expect),
        "found %s => %s", key, value
    );
}


void 
hash_hasnt(
    hemp_hash_p table, 
    hemp_str_p  key
) {
    hemp_str_p value = hemp_hash_fetch_string(table, key);
    ok( ! value, "no entry for %s", key);
}


void 
hash_has_keylen(
    hemp_hash_p table, 
    hemp_str_p  key, 
    hemp_size_t length,
    hemp_str_p  expect
) {
    hemp_value_t value = hemp_hash_fetch_keylen(table, key, length);

    if (hemp_is_missing(value)) {
        fail("not found: %s (length:%d)", key, length);
    }
    else {
        hemp_str_p str = hemp_val_str(value); 
        ok( 
            str && hemp_string_eq(str, expect),
            "found %s (length: %d) => %s", key, length, str
        );
    }
}

void 
hash_hasnt_keylen(
    hemp_hash_p table, 
    hemp_str_p  key,
    hemp_size_t length
) {
    hemp_value_t value = hemp_hash_fetch_keylen(table, key, length);
    ok( hemp_is_missing(value), "no entry for %s (length: %d)", key, length);
}


void 
hash_set(
    hemp_hash_p table, 
    hemp_str_p  key, 
    hemp_str_p  value
) {
    ok(
        hemp_hash_store_string(table, key, value),
        "set %s to %s", key, value
    );
}
