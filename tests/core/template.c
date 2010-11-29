#include <hemp/test.h>


void test_template();


int
main(
    int  argc, 
    char **argv, 
    char **env
) {
    plan(5);
    test_template();
    return done();
}


void test_template() {
    hemp_hemp hemp = hemp_new();
    ok( hemp, "created hemp object" );

    hemp_language_instance(hemp, "tt3");
    
    hemp_debug_msg("got tt3 langauge\n"); 

    hemp_template tmpl = hemp_template_instance(
        hemp, HEMP_TT3, HEMP_TEXT, "Hello World!"
    );
    ok( tmpl , "created template" );

    hemp_bool scanned = hemp_template_scan(tmpl);
    ok( scanned, "scanned template" );

    hemp_template tmpl2 = hemp_template_instance(
        hemp, HEMP_TT3, HEMP_TEXT, "Hello World!"
    );
    ok( tmpl2 , "created same template" );

    hemp_element first = hemp_template_tokens(tmpl);
    ok( first, "got first token from template" );
    
    hemp_free(hemp);
}


