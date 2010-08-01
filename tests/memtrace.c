#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hemp.h"
#include "tap.h"


#define STR1     "debug test program"
#define STR2     "by Andy Wardley"

void test_memtrace();

int
main(int argc, char **argv, char **env)
{
    plan_tests(1);
    test_memtrace();
#if DEBUG & DEBUG_MEM
//    hemp_mem_debug_report(HEMP_TRUE);
    hemp_mem_debug_cleanup();
#endif
    return exit_status();
}

void test_memtrace() {
    char *foo;
    char *bar;

    foo = (char *) hemp_mem_init(strlen(STR1) + 1);
    bar = (char *) hemp_mem_init(strlen(STR2) + 1);
    strcpy(foo, STR1);
    strcpy(bar, STR2);

    hemp_mem_debug_report(HEMP_TRUE);
    hemp_mem_free(foo);

    hemp_mem_debug_report(HEMP_TRUE);
    hemp_mem_free(bar);

    hemp_mem_debug_report(HEMP_TRUE);
    
    ok( 1, "ran some memory tests" );
}

