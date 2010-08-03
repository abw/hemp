#include <hemp.h>
#include <tap.h>

void test_slab();

int
main(int argc, char **argv, char **env)
{
    plan_tests(3);
    test_slab();
    hemp_mem_trace_ok();
    return exit_status();
}


void test_slab() {
    hemp_slab_p slab = hemp_slab_init(100);
    ok( slab, "created slab of %d bytes", slab->size );
    hemp_slab_free(slab);
    pass("released slab");
}

