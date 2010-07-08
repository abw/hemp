#include <stdio.h>
#include "hemp.h"

void test_dialect();

int
main(int argc, char **argv, char **env)
{
    test_dialect();
    return 0;
}

void test_dialect() {
    hemp_dialect_t dialect = hemp_dialect_init("testing");
    
    dialect
        ? pass("created %s dialect", dialect->name)
        : fail("failed to created testing dialect");

    hemp_dialect_free(dialect);

    dialect = hemp_dialect_tt3();

    dialect
        ? pass("created %s dialect", dialect->name)
        : fail("failed to created TT3 dialect");

    hemp_dialect_free(dialect);
}

