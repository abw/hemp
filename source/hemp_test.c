#include <stdio.h>
#include <assert.h>
#include "config.h"
#include "hemp.h"

void test_debug();
void test_memory();
void test_slab();
void test_ptree();
//void test_tags();
//void test_scanner();

int
main(int argc, char **argv, char **env)
{
/*
    test_debug();
    test_memory();
    test_slab();
    test_ptree();
    test_tags();
    test_scanner();
*/

    return 0;
}


void test_debug() {
    debug_red("This is red\n");
    debug_green("This is green\n");
    debug_yellow("This is yellow\n");
    debug_blue("This is blue\n");
    debug_magenta("This is magenta\n");
    debug_cyan("This is cyan\n");
    debug("Turning debugging off for a moment\n");
    debug_off();
    debug("!!! FAIL !!! - You shouldn't see this message\n");
    debug_on();
    debug("Turned debugging back on\n");
}


void test_memory() {
    char *a = hemp_mem_init(100);
    sprintf(a, "Hello %s!\n", "World");
    debug_green(a);

    char *b = hemp_mem_init(100);
    hemp_mem_copy(a, b, 100);
    debug_green(b);
    
    hemp_mem_free(a);
    hemp_mem_free(b);
}


void test_slab() {
    hemp_slab_t slab = hemp_slab_init(100);
    
    if (slab) {
        pass("OK - created slab of %d bytes", slab->size);
        hemp_slab_free(slab);
    }
    pass("released slab");
}


void test_ptree() {
    hemp_ptree_t ptree;
    hemp_pnode_t pnode;
    int i;

    debug("test_ptree()\n");
    
    ptree = hemp_ptree_init(8);

    pass("created ptree at %p", ptree);

    hemp_ptree_insert(ptree, "for", "tt.command.for");
    hemp_ptree_insert(ptree, "foreach", "tt.command.foreach");
    hemp_ptree_insert(ptree, "fill", "tt.command.fill");
    hemp_ptree_insert(ptree, "form", "tt.command.form");
    hemp_ptree_insert(ptree, "bar", "tt.example.bar");
    hemp_ptree_insert(ptree, "baz", "tt.example.baz");
    hemp_ptree_insert(ptree, "happy", "tt.silly.happy");
    hemp_ptree_insert(ptree, "hippy", "tt.silly.hippy");
    hemp_ptree_insert(ptree, "brazen", "tt.silly.brazen");
    hemp_ptree_insert(ptree, "hiphiphooray", "tt.silly.hiphip");
    
    hemp_ptree_dump(ptree);
    
    hemp_ptree_free(ptree);
//    for (i = 0; i < 10; i++) {
//        pnode = attic_ptree_node(ptree);
//        debug("%d: %p\n", i, pnode);
//    }
    
}




//void test_tags() {
//    debug("test_tags()\n");
//    hemp_tagset_t tagset = make_tagset();
//    hemp_tagset_dump(tagset);
//    hemp_tagset_free(tagset);
//}


/*
void test_scanner() {
    debug("test_scanner()\n");
    hemp_tempate_t tmpl = hemp_template(
        HEMP_TEXT,
        "The [#cat#] %sat\n%%on $the mat\n%and [%shat%]\nand it was [? phat ?] Oh ${my}!\n[foo] and [% bar %] then [? baz ?] but \nnot [! wibble !] or [frusset pouch]\nThe End"
    );
    hemp_tagset_t tagset = make_tagset();
    hemp_tagset_dump(tagset);

    hemp_scan_text(text, tagset);

    hemp_tagset_free(tagset);
}
*/

