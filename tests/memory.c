#include "hemp.h"
#include "tap.h"

void test_memory();

int
main(int argc, char **argv, char **env)
{
    plan_tests(7);
    test_memory();
    hemp_mem_trace_ok();
    return exit_status();
}


void test_memory() {
    char *a = hemp_mem_alloc(100);
    sprintf(a, "Hello %s!\n", "World");
    ok( a, "Allocated memory" );

    char *b = hemp_mem_alloc(100);
    ok( b, "Allocated more memory" );

    hemp_mem_copy(a, b, 100);
    ok( hemp_cstr_eq(a, b), "strings match" );

    char *c = hemp_cstr_copy(b);
    ok( c, "duplicated string" );
    ok( hemp_cstr_eq(b, c), "strings match" );
    
    hemp_mem_free(a);
    hemp_mem_free(b);
    hemp_mem_free(c);

    pass("freed memory");
}
