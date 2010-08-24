#include <hemp/test.h>

void test_context();

int main(
    int argc, char **argv, char **env
) {
    plan(9);
    test_context();

//  fail("TMP: bailing out early for development purposes");
    return done();
}


void test_context() {
    hemp_p hemp = hemp_init();
    ok( hemp, "created hemp object" );

    hemp_context_p context = hemp_context(hemp);
    ok( context, "created hemp context" );
    ok( context->hemp == hemp, "context->hemp points at hemp" );

    ok( hemp_context_set_number(context, "pi", 3.14159), "set pi" );
    eq( hemp_context_get_number(context, "pi"), 3.14159, "get pi" );

    ok( hemp_context_set_integer(context, "answer", 42), "set answer" );
    eq( hemp_context_get_integer(context, "answer"), 42, "get answer" );

    ok( hemp_context_set_string(context, "message", "hello world"), "set message" );
    is( hemp_context_get_string(context, "message"), "hello world", "get message" );

    hemp_context_free(context);
    hemp_free(hemp);
}

