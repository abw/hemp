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
    Hemp hemp = hemp_new();
    ok( hemp, "created hemp object" );

    HempString version = hemp_version();
    ok( version, "got hemp version: %s", version );
    
    hemp_free(hemp);
    ok( hemp, "released hemp object" );
}


