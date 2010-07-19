#include <stdio.h>
#include "hemp.h"
#include "tap.h"

void test_tree();
void tree_get(hemp_ptree_t, char *);
void tree_set(hemp_ptree_t tree, char *key, char *value);
void tree_has(hemp_ptree_t tree, char *key, char *expect);
void tree_hasnt(hemp_ptree_t tree, char *key);


int
main(int argc, char **argv, char **env)
{
    plan_tests(9);
    test_tree();
    return exit_status();
}

void test_tree() {
    hemp_ptree_t tree = hemp_ptree_init(32);

    tree_set(tree, "msg", "hello world");
    tree_has(tree, "msg", "hello world");

    tree_set(tree, "foo", "The foo item");
    tree_has(tree, "foo", "The foo item");

    tree_hasnt(tree, "f");
    tree_hasnt(tree, "fo");
    tree_hasnt(tree, "food");
    tree_hasnt(tree, "bar");
    
    tree_set(tree, "bar", "The bar item");
    tree_has(tree, "bar", "The bar item");

//    hemp_ptree_dump(tree);
    tree_set(tree, "from",       "keyword:from");
    tree_has(tree, "from",       "keyword:from");

    tree_set(tree, "for",       "keyword:for");
    tree_has(tree, "for",       "keyword:for");

    tree_set(tree, "foreach",   "keyword:foreach");
    tree_has(tree, "foreach",   "keyword:foreach");
    
    tree_has(tree, "foo", "The foo item");

    tree_hasnt(tree, "f");
    tree_hasnt(tree, "forever");
    tree_hasnt(tree, "foreachs");

    hemp_ptree_dump(tree);
    hemp_ptree_free(tree);
}


void 
tree_has(hemp_ptree_t tree, char *key, char *expect) 
{
    hemp_ptr_t value = hemp_ptree_fetch(tree, key);
    debug("GOT VALUE: %s\n", value);
    ok( 
        value && hemp_cstr_eq(value, expect),
        "found %s => %s", key, value
    );
}


void 
tree_hasnt(hemp_ptree_t tree, char *key) 
{
    hemp_ptr_t value = hemp_ptree_fetch(tree, key);
    ok( ! value, "no entry for %s", key);
}


void 
tree_set(hemp_ptree_t tree, char *key, char *value) 
{
    ok(
        hemp_ptree_store(tree, key, value),
        "set %s to %s", key, value
    );
}
