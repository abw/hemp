#include <hemp/test.h>


void test_fragments();

int main(
    int argc, char **argv, char **env
) {
    plan(4);
    test_fragments();
    return done();
}

void test_fragments() {
    Hemp hemp = hemp_new();
    ok( hemp, "created hemp object" );
    
    HempFragments fragments = hemp_fragments_new(NULL, 0);
    ok( fragments, "created fragments" );
        
    HempFragment eof = hemp_fragments_add_eof(fragments, 0);
    ok( eof, "got EOF fragment" );
    ok( hemp_has_flag(eof, HEMP_BE_EOF), "HEMP_BE_EOF is set" );

    hemp_fragments_free(fragments);
    hemp_free(hemp);
}
