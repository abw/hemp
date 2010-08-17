#include <hemp/test.h>


void        test_dialect();
void        test_dialect_registration();
hemp_bool_t dummy_dialect_builder(hemp_p);


int main(
    int argc, char **argv, char **env
) {
    plan(3);
    test_dialect();
    return done();
}


void test_dialect() {
    hemp_p hemp = hemp_init();
    ok( hemp, "created hemp object" );

    hemp_language(hemp, "tt3");

    hemp_dialect_p dialect = hemp_dialect(hemp, "tt3");
    ok( dialect, "fetched %s dialect", dialect->name );

    dialect = hemp_dialect(hemp, "tt3");
    ok( dialect, "fetched %s dialect again", dialect->name );

    hemp_free(hemp);
}


void test_dialect_registration() {
    hemp_p hemp = hemp_init();
//    ok( 
//        hemp_register_dialect(hemp, "test1", &dummy_dialect_builder),
//        "registered test1 dialect"
//    );
    hemp_free(hemp);
}

hemp_bool_t 
dummy_dialect_builder(
    hemp_p hemp
) {
    hemp_debug_cyan("dummy dialect builder\n");
}
