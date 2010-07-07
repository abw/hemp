#include <stdio.h>
#include "hemp.h"

void test_debug();
void test_memory();

int
main(int argc, char **argv, char **env)
{
    test_debug();
    test_memory();
    return 0;
}


void test_debug() {
    debug_red("This is red\n");
    debug_green("This is green\n");
    debug_yellow("This is yellow\n");
    debug_blue("This is blue\n");
    debug_magenta("This is magenta\n");
    debug_cyan("This is cyan\n");
}

void test_memory() {
    char *a = hemp_mem_init(100);
    sprintf(a, "Hello %s!\n", "World");
    debug_green(a);

    char *b = hemp_mem_copy(a, b, 100);
    debug_green(b);
}
    