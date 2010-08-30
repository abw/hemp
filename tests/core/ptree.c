#include <hemp/test.h>

void test_tree();
void tree_get(hemp_ptree_p, char *);
void tree_set(hemp_ptree_p tree, char *key, char *value);
void tree_has(hemp_ptree_p tree, char *key, char *expect);
void tree_hasnt(hemp_ptree_p tree, char *key);


int main(
    int argc, char **argv, char **env
) {
    plan(186);
    test_tree();
    return done();
}

void test_tree() {
    hemp_ptree_p tree = hemp_ptree_init(32);

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

    hemp_list_p list = hemp_string_split(
        "alpha bravo charlie delta echo foxtrot golf hotel india juliet "
        "kilo lima mike november oscar papa quebec romeo sierra tango "
        "umbrella victor whiskey x-ray yankee zulu "
        "amazing blue cream dip evensong frusset goal hooters iphone "
        "juggler kiss love mother naughty oscillate pouch quality rub "
        "sexy touch underpants velvet wibble xanadu yubble zoom"
        "+ ++ - -- * % -> => = == <= >= < > <=> . , ; ' | [ ] { } ${ $ .. "  
        "in and or not ",
        " "
    );
    list->cleaner = &hemp_list_each_free;
    int i;
    char *word;
    
    for (i = 0; i < list->length; i++) {
        word = hemp_list_item_string(list, i);
        tree_set(tree, word, word);
    }

    for (i = 0; i < list->length; i++) {
        word = hemp_list_item_string(list, i);
        tree_has(tree, word, word);
    }

/*
    hemp_ptree_dump(tree);
    printf("*** MAX DEPTH: %d\n", hemp_ptree_max_depth(tree));
    printf("*** PTREE WIDTH: %d\n", HEMP_PTREE_WIDTH);
    printf("*** TOTAL NODES: %d\n", hemp_ptree_count_nodes(tree));
*/

    hemp_ptree_free(tree);
    hemp_list_free(list);
}


void 
tree_has(
    hemp_ptree_p tree, char *key, char *expect
) {
    hemp_mem_p value = hemp_ptree_fetch(tree, key);
    ok( 
        value && hemp_string_eq(value, expect),
        "found %s => %s", key, value
    );
}


void 
tree_hasnt(
    hemp_ptree_p tree, char *key
) {
    hemp_mem_p value = hemp_ptree_fetch(tree, key);
    ok( ! value, "no entry for %s", key);
}


void 
tree_set(
    hemp_ptree_p tree, char *key, char *value
) {
    ok(
        hemp_ptree_store(tree, key, value),
        "set %s to %s", key, value
    );
}
