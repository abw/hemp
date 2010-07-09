#include <stdio.h>
#include <assert.h>
#include "config.h"
#include "hemp.h"

void test_debug();
void test_ptree();

int
main(int argc, char **argv, char **env)
{
/*
    test_ptree();
*/
    test_debug();

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
    
    debug_mem("Memory debugging\n");
    debug_load("Module loading debugging\n");
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



