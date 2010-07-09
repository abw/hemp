#include <stdio.h>
#include "hemp.h"
#include "tap.h"

void test_slab();

int
main(int argc, char **argv, char **env)
{
    plan_tests(2);
    test_slab();
    return exit_status();
}


void test_slab() {
    hemp_slab_t slab = hemp_slab_init(100);
    ok( slab, "created slab of %d bytes", slab->size );
    hemp_slab_free(slab);
    pass("released slab");
}

