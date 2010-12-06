#include <hemp/test.h>

void test_unplugged();


int
main(
    int  argc, 
    char **argv, 
    char **env
) {
    plan(3);
    test_unplugged();
    return done();
}


void test_unplugged() {
    hemp_hemp       hemp = hemp_new();
    hemp_template   template;
    hemp_context    context;
    hemp_text       output;
    ok( hemp, "created hemp object" );

    hemp_language_instance(hemp, "tt3");
    hemp_debug_msg("got tt3 langauge\n"); 

    HEMP_TRY;
        template = hemp_template_instance(
            hemp, "tt3.unplugged", HEMP_TEXT, "a=10; 'a is '; a"
        );
        ok( template , "created template" );

        context = hemp_context_new(hemp);
        output  = hemp_template_render(template, context);

        is( output->string, "a is 10", "correctly rendered template" );
//      hemp_debug_msg("OUTPUT: %s\n", output->string);
        hemp_context_free(context);
        hemp_text_free(output);

    HEMP_CATCH_ALL;
        fail( "Caught error: %s", hemp->error->message );

    HEMP_END;

    hemp_free(hemp);
}


