#include <hemp/test.h>

void test_memory();

int
main(int argc, char **argv, char **env)
{
    plan(6);
    test_memory();
    return done();
}


void test_memory() {
    char *a = hemp_mem_alloc(100);
    sprintf(a, "Hello %s!\n", "World");
    ok( a, "Allocated memory" );

    char *b = hemp_mem_alloc(100);
    ok( b, "Allocated more memory" );

    hemp_mem_copy(a, b, 100);
    ok( hemp_string_eq(a, b), "strings match" );

    char *c = hemp_string_copy(b);
    ok( c, "duplicated string" );
    ok( hemp_string_eq(b, c), "strings match" );
    
    hemp_mem_free(a);
    hemp_mem_free(b);
    hemp_mem_free(c);

    pass("freed memory");
}
