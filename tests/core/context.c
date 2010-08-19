#include <hemp/test.h>

void test_context();

int main(
    int argc, char **argv, char **env
) {
    plan(3);
    test_context();
    return done();
}


void test_context() {
    hemp_p hemp = hemp_init();
    ok( hemp, "created hemp object" );

    hemp_context_p context = hemp_context(hemp);
    ok( context, "created hemp context" );

    ok( context->hemp == hemp, "context->hemp points at hemp" );

    hemp_context_free(context);
    hemp_free(hemp);
}

