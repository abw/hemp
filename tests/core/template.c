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
    hemp_p hemp = hemp_init();
    ok( hemp, "created hemp object" );

    hemp_language(hemp, "tt3");

    hemp_template_p tmpl = hemp_template(
        hemp, HEMP_TT3, HEMP_TEXT, "Hello World!"
    );
    ok( tmpl , "created template" );

    hemp_bool_t scanned = hemp_template_scan(tmpl);
    ok( scanned, "scanned template" );

    hemp_template_p tmpl2 = hemp_template(
        hemp, HEMP_TT3, HEMP_TEXT, "Hello World!"
    );
    ok( tmpl2 , "created same template" );

    hemp_element_p first = hemp_template_tokens(tmpl);
    ok( first, "got first token from template" );
    
    hemp_free(hemp);
}


