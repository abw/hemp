#include <hemp.h>
#include <tap.h>


void test_tt3();


int
main(
    int  argc, 
    char **argv, 
    char **env
) {
    plan_tests(5);
    test_tt3();
    hemp_mem_trace_ok();
    return exit_status();
}


void test_tt3() {
    hemp_p hemp = hemp_init();
    ok( hemp, "created hemp object" );

    HEMP_TRY;
        hemp_language_p lang = hemp_language(hemp, "tt3");
        ok( lang, "using %s version %0.2f", lang->name, lang->version);

        hemp_dialect_p dial = hemp_dialect(hemp, "tt3");
        ok( dial, "got %s dialect", dial->name );

        hemp_template_p tmpl = hemp_template(
            hemp, HEMP_TT3, HEMP_TEXT, "Hello World!"
            );
        ok( tmpl , "created template" );

    HEMP_CATCH_ALL;
        fail( "Caught error: %s", hemp->error->message );

    HEMP_END;

    hemp_free(hemp);
}


