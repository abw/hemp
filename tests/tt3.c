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
    Hemp hemp = hemp_new();
    ok( hemp, "created hemp object" );

    HEMP_TRY;
        HempLanguage lang = hemp_language_instance(hemp, "tt3");
        ok( lang, "using %s version %0.2f", lang->name, lang->version);

        HempDialect dial = hemp_dialect_instance(hemp, "tt3");
        ok( dial, "got %s dialect", dial->name );

        HempDocument document = hemp_document_instance(
            hemp, HEMP_TT3, HEMP_TEXT, "Hello World!"
            );
        ok( document , "created document" );

    HEMP_CATCH_ALL;
        fail( "Caught error: %s", hemp->error->message );

    HEMP_END;

    hemp_free(hemp);
}


