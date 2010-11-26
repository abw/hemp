#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hemp.h"
#include "hemp/debug.h"

#define ITERATIONS 1000000

struct hemp_test {
    char *name;
    void (* hash)();
    void (* tree)();
};

void run_tests();
void run_test(struct hemp_test *);
void hash_store();
void tree_store();
void hash_fetch();
void tree_fetch();


int
main(int argc, char **argv, char **env)
{
    run_tests();
    return 0;
}


void run_tests() {
    struct hemp_test store = {
        "store", 
        &hash_store,
        &tree_store
    };
//    run_test(&store);

    struct hemp_test fetch = {
        "fetch", 
        &hash_fetch,
        &tree_fetch
    };
    run_test(&fetch);
}


void run_test(struct hemp_test *test) {
    clock_t t1, t2, t3;
    double hash_time, tree_time;
    
    t1 = clock();
    test->hash();
    t2 = clock();
    test->tree();
    t3 = clock();

    hash_time = ((double) (t2 - t1)) / CLOCKS_PER_SEC;
    tree_time = ((double) (t3 - t2)) / CLOCKS_PER_SEC;
    printf("%s\n  hash: %.4lf (%ld)\n  tree: %.4lf (%ld)\n", test->name, hash_time, (t2 - t1), tree_time, (t3 - t2));
}



void test_store(
    hemp_ptr_t store,
    hemp_ptr_t (storefn)(),
    int ntests
) {
    char key[100];
    int keylen;
    char *value = "dummy value";

    while (ntests--) {
        for (keylen = 1; keylen < 20; keylen++) {
            int k = 0;
            while (k < keylen) {
                key[k++] = '0' + ((int) random() % 77);
            }
            key[k] = '\0';
/*          printf("key(%d): %s\n", keylen, key); */

            storefn(store, key, value);
        }
    }
}


void hash_store() {
    hemp_hash_t hash = hemp_hash_init();
    test_store(hash, &hemp_hash_store, ITERATIONS);
    hemp_hash_free(hash);
}


void tree_store() {
    hemp_ptree_t tree = hemp_ptree_new(32);
    test_store(tree, &hemp_ptree_store, ITERATIONS);
    hemp_ptree_free(tree);
}



void test_fetch_prep(
    hemp_ptr_t store,
    hemp_ptr_t (storefn)(),
    int ntests
) {
    char *key;
    int keylen;
    char *value = "dummy value";

    while (ntests--) {
        for (keylen = 1; keylen < 20; keylen++) {
            key = malloc(keylen + 1);
            int k = 0;
            while (k < keylen) {
                key[k++] = '0' + ((int) random() % 77);
            }
            key[k] = '\0';
            storefn(store, key, value);
        }
    }
}

void test_fetch(
    hemp_ptr_t store,
    hemp_ptr_t (fetchfn)(),
    int ntests
) {
    char key[100];
    int keylen;
    char *value = "dummy value";

    while (ntests--) {
        for (keylen = 1; keylen < 20; keylen++) {
            int k = 0;
            while (k < keylen) {
                key[k++] = '0' + ((int) random() % 77);
            }
            key[k] = '\0';
/*          printf("key(%d): %s\n", keylen, key); */
            fetchfn(store, key);
        }
    }
}

void hash_fetch() {
    hemp_hash_t hash = hemp_hash_init();
    test_fetch_prep(hash, &hemp_hash_store, 20);
    test_fetch(hash, &hemp_hash_fetch, ITERATIONS);
    hemp_hash_free(hash);
}


void tree_fetch() {
    hemp_ptree_t tree = hemp_ptree_new(32);
    test_fetch_prep(tree, &hemp_ptree_store, 20);
    test_fetch(tree, &hemp_ptree_fetch, ITERATIONS);
    hemp_ptree_free(tree);
}
