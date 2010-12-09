#include <hemp/test.h>


void test_document();


int
main(
    int  argc, 
    char **argv, 
    char **env
) {
    plan(5);
    test_document();
    return done();
}


void test_document() {
    hemp_hemp hemp = hemp_new();
    ok( hemp, "created hemp object" );

    hemp_language_instance(hemp, "tt3");
    
    hemp_debug_msg("got tt3 langauge\n"); 

    hemp_document document = hemp_document_instance(
        hemp, HEMP_TT3, HEMP_TEXT, "Hello World!"
    );
    ok( document , "created document" );

    hemp_bool scanned = hemp_document_scan(document);
    ok( scanned, "scanned document" );

    hemp_document document2 = hemp_document_instance(
        hemp, HEMP_TT3, HEMP_TEXT, "Hello World!"
    );
    ok( document2 , "created same document" );

    hemp_fragment first = hemp_document_tokens(document);
    ok( first, "got first token from document" );
    
    hemp_free(hemp);
}


