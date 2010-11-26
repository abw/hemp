#include <hemp/test.h>

void test_slab();

int main(
    int argc, char **argv, char **env
) {
    plan(2);
    test_slab();
    return done();
}


void test_slab() {
    hemp_slab slab = hemp_slab_new(100);
    ok( slab, "created slab of %d bytes", slab->size );
    hemp_slab_free(slab);
    pass("released slab");
}

