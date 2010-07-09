#include <stdio.h>
#include "hemp.h"
#include "tap.h"


void        test_dialect();
void        test_dialect_registration();
hemp_bool_t dummy_dialect_builder(hemp_t);


int
main(int argc, char **argv, char **env)
{
    plan_tests(2);
    test_dialect();
    return exit_status();
}


void test_dialect() {
    hemp_dialect_t dialect = hemp_dialect_init("testing");
    ok( dialect, "created %s dialect", dialect->name );
    hemp_dialect_free(dialect);

    dialect = hemp_dialect_tt3();
    ok( dialect, "created %s dialect", dialect->name );
    hemp_dialect_free(dialect);
}


void test_dialect_registration() {
    hemp_t hemp = hemp_init();
    ok( 
        hemp_register_dialect(hemp, "test1", &dummy_dialect_builder),
        "registered test1 dialect"
    );
    hemp_free(hemp);
}

hemp_bool_t 
dummy_dialect_builder(
    hemp_t hemp
) {
    debug_cyan("dummy dialect builder\n");
}

