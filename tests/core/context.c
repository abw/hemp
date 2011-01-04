#include <hemp/test.h>

void test_context();

int main(
    int argc, char **argv, char **env
) {
    plan(10);
    test_context();
    return done();
}


void test_context() {
    Hemp hemp = hemp_new();
    ok( hemp, "created hemp object" );

    HempContext context = hemp_context(hemp);
    ok( context, "created hemp context" );
    ok( context->hemp == hemp, "context->hemp points at hemp" );

    ok( hemp_context_set_number(context, "pi", 3.14159), "set pi" );
    eq( hemp_context_get_number(context, "pi"), 3.14159, "get pi" );

    ok( hemp_context_set_integer(context, "answer", 42), "set answer" );
    eq( hemp_context_get_integer(context, "answer"), 42, "get answer" );

    ok( hemp_context_set_string(context, "message", "hello world"), "set message" );
    is( hemp_context_get_string(context, "message"), "hello world", "get message" );

    int n;
    for (n = 0; n < 10; n++) {
        /* create a bunch of temporary items - the context should 
         * automagically clean these up when it is free.
         */
        HempList list = hemp_context_tmp_list(context);
        HempText text = hemp_context_tmp_text(context);
        HempCode code = hemp_context_tmp_code(context);
        hemp_text_append_string(text, "hello world!");

        /* prevent "unused variable" warnings */
        HEMP_UNUSED(list);
        HEMP_UNUSED(text);
        HEMP_UNUSED(code);
        
    }

    HempFrame frame = hemp_context_enter(context, NULL);
    ok( frame, "got frame" );

    hemp_context_free(context);
    hemp_free(hemp);
}

