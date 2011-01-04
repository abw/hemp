#include <hemp/test.h>

void test_hash();
void test_hash_methods();
void hash_get(HempHash, char *);
void hash_set(HempHash table, HempString, HempString);
void hash_has(HempHash table, HempString, HempString);
void hash_hasnt(HempHash table, HempString);
void hash_has_keylen(HempHash table, HempString, HempSize, HempString);
void hash_hasnt_keylen(HempHash table, HempString, HempSize);
void hash_index_same(HempString, HempSize, HempString, HempSize);
void hash_index_different(HempString, HempSize, HempString, HempSize);
void hash_keys_do_match(HempString, HempString, HempSize);
void hash_keys_no_match(HempString, HempString, HempSize);


int main(
    int argc, char **argv, char **env
) {
    plan(28);
    test_hash();
    test_hash_methods();
    return done();
}


void test_hash() {
    HempHash table = hemp_hash_new();
    HempHash child = hemp_hash_new();
    

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
    HempString  hash_key,
    HempString  search_key,
    HempSize length
) {
    ok(
        hemp_hash_key_match(hash_key, search_key, length),
        "match hash key [%s] with search key [%s] length [%d]", 
        hash_key, search_key, length
    );
}


void hash_keys_no_match(
    HempString  hash_key,
    HempString  search_key,
    HempSize length
) {
    ok(
        ! hemp_hash_key_match(hash_key, search_key, length),
        "no hash key [%s] with search key [%s] length [%d]", 
        hash_key, search_key, length
    );
}

void hash_index_same(
    HempString  key1,
    HempSize len1,
    HempString  key2,
    HempSize len2
) {
    HempSize hash1 = hemp_hash_function(key1, len1);
    HempSize hash2 = hemp_hash_function(key2, len2);
    ok( hash1 == hash2, "hash(%s[%d]) == hash(%s[%d])", key1, len1, key2, len2); 
}


void hash_index_different(
    HempString  key1,
    HempSize len1,
    HempString  key2,
    HempSize len2
) {
    HempSize hash1 = hemp_hash_function(key1, len1);
    HempSize hash2 = hemp_hash_function(key2, len2);
    ok( hash1 != hash2, "hash(%s[%d]) != hash(%s[%d])", key1, len1, key2, len2); 
}


void 
hash_has(
    HempHash table, 
    HempString  key, 
    HempString  expect
) {
    HempString value = hemp_hash_fetch_string(table, key);
    ok( 
        value && hemp_string_eq(value, expect),
        "found %s => %s", key, value
    );
}


void 
hash_hasnt(
    HempHash table, 
    HempString  key
) {
    HempString value = hemp_hash_fetch_string(table, key);
    ok( ! value, "no entry for %s", key);
}


void 
hash_has_keylen(
    HempHash table, 
    HempString  key, 
    HempSize length,
    HempString  expect
) {
    HempValue value = hemp_hash_fetch_keylen(table, key, length);

    if (hemp_is_missing(value)) {
        fail("not found: %s (length:%d)", key, length);
    }
    else {
        HempString str = hemp_val_str(value); 
        ok( 
            str && hemp_string_eq(str, expect),
            "found %s (length: %d) => %s", key, length, str
        );
    }
}

void 
hash_hasnt_keylen(
    HempHash table, 
    HempString  key,
    HempSize length
) {
    HempValue value = hemp_hash_fetch_keylen(table, key, length);
    ok( hemp_is_missing(value), "no entry for %s (length: %d)", key, length);
}


void 
hash_set(
    HempHash table, 
    HempString  key, 
    HempString  value
) {
    ok(
        hemp_hash_store_string(table, key, value),
        "set %s to %s", key, value
    );
}


void test_hash_methods() {
    Hemp       hemp    = hemp_new();
    HempHash  hash    = hemp_hash_new();
//  HempText  text    = hemp_text_from_string("Hello World!");
    HempValue value   = hemp_hash_val(hash);
    HempContext  context = hemp_context(hemp); 
    
    hemp_hash_store_string(hash, "message", "Hello World");
    hemp_hash_store_integer(hash, "answer", 42);
    hemp_hash_store_number(hash, "pi", 3.14159);
    
    ok( hemp_is_hash(value), "value is a hash" );
    
    HempValue length = hemp_send(value, "length", context, NULL);
    ok( hemp_is_defined(length), "got defined length" );
    ok( hemp_is_integer(length), "got an integer length" );
    eq( hemp_val_int(length), 3, "hash length is 3" );

    HempValue htext = hemp_send(value, "text", context, NULL);
    ok( hemp_is_text(htext), "got hash as text" );

    hemp_context_free(context);
    hemp_hash_free(hash);
//    hemp_text_free( hemp_val_text(htext) );
    hemp_free(hemp);
}