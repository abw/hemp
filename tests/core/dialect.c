#include <hemp/test.h>


void        test_dialect();
void        test_dialect_registration();
HempBool dummy_dialect_builder(Hemp);


int main(
    int argc, char **argv, char **env
) {
    plan(3);
    test_dialect();
    return done();
}


void test_dialect() {
    Hemp hemp = hemp_new();
    ok( hemp, "created hemp object" );

    hemp_language(hemp, "tt3");

    HempDialect dialect = hemp_dialect(hemp, "tt3");
    ok( dialect, "fetched %s dialect", dialect->name );

    dialect = hemp_dialect(hemp, "tt3");
    ok( dialect, "fetched %s dialect again", dialect->name );

    hemp_free(hemp);
}


void test_dialect_registration() {
    Hemp hemp = hemp_new();
//    ok( 
//        hemp_register_dialect(hemp, "test1", &dummy_dialect_builder),
//        "registered test1 dialect"
//    );
    hemp_free(hemp);
}

HempBool 
dummy_dialect_builder(
    Hemp hemp
) {
    hemp_debug_cyan("dummy dialect builder\n");
    return HEMP_TRUE;
}

