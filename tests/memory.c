#include <stdio.h>
#include "hemp.h"
#include "tap.h"

void test_memory();

int
main(int argc, char **argv, char **env)
{
    plan_tests(4);
    test_memory();
    return exit_status();
}


void test_memory() {
    char *a = hemp_mem_init(100);
    sprintf(a, "Hello %s!\n", "World");
    ok( a, "Allocated memory" );

    char *b = hemp_mem_init(100);
    ok( b, "Allocated more memory" );

    hemp_mem_copy(a, b, 100);
    ok( hemp_str_eq(a, b), "strings match" );
    
    hemp_mem_free(a);
    hemp_mem_free(b);

    pass("freed memory");
}
