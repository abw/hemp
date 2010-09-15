#include <hemp/test.h>

void test_hemp();

int
main(int argc, char **argv, char **env)
{
    plan(3);
    test_hemp();
    return done();
}

void test_hemp() {
    hemp_p hemp = hemp_init();
    ok( hemp, "created hemp object" );

    hemp_str_p version = hemp_version();
    ok( version, "got hemp version: %s", version );
    
    hemp_free(hemp);
    ok( hemp, "released hemp object" );
}


