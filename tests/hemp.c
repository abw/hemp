#include <stdio.h>
#include "hemp.h"
#include "tap.h"

void test_hemp();

int
main(int argc, char **argv, char **env)
{
    plan_tests(2);
    test_hemp();
    return exit_status();
}

void test_hemp() {
    hemp_t hemp = hemp_init();
    ok( hemp, "created hemp object" );
    hemp_free(hemp);
    ok( hemp, "released hemp object" );
}
