#include <hemp/test.h>


void test_punctuation();


int main(
    int argc, char **argv, char **env
) {
    plan(9);
    test_punctuation();
    return done();
}


void test_punctuation() {
    HempString text = "DUMMY TEXT";

    Hemp hemp = hemp_new();
    ok( hemp, "created hemp" );

    HempFragments fragments = hemp_fragments_new(NULL, 0);
    ok( fragments, "created fragments" );

    HempElement space      = NULL, 
                comma      = NULL, 
                semicolon  = NULL, 
                terminator = NULL;
    
    HEMP_TRY;
        space = hemp_element(hemp, "hemp.space", " ", NULL);
        ok( space, "got %s symbol", space->name );

        comma = hemp_element(hemp, "hemp.delimiter", ",", NULL);
        ok( comma, "got %s symbol", comma->name );

        semicolon = hemp_element(hemp, "hemp.separator", ";", NULL);
        ok( semicolon, "got %s symbol", semicolon->name );

        terminator = hemp_element(hemp, "hemp.terminator", ".", NULL);
        ok( terminator, "got %s symbol", terminator->name );

    HEMP_CATCH_ALL;
        fail("unexpected error: %s", hemp->error->message);

    HEMP_END;

    hemp_fragments_add_fragment(fragments, space,       text, 0, 0);
    hemp_fragments_add_fragment(fragments, comma,       text, 0, 0);
    hemp_fragments_add_fragment(fragments, space,       text, 0, 0);
    hemp_fragments_add_fragment(fragments, comma,       text, 0, 0);
    /* skipping delimiters should stop here */
    hemp_fragments_add_fragment(fragments, semicolon,   text, 0, 0);
    hemp_fragments_add_fragment(fragments, space,       text, 0, 0);
    hemp_fragments_add_fragment(fragments, semicolon,   text, 0, 0);
    hemp_fragments_add_fragment(fragments, comma,       text, 0, 0);
    hemp_fragments_add_fragment(fragments, space,       text, 0, 0);
    /* skipping separators should stop here */
    hemp_fragments_add_fragment(fragments, terminator,  text, 0, 0);
    hemp_fragments_add_fragment(fragments, space,       text, 0, 0);
    hemp_fragments_add_fragment(fragments, comma,       text, 0, 0);
    hemp_fragments_add_fragment(fragments, space,       text, 0, 0);
    hemp_fragments_add_fragment(fragments, semicolon,   text, 0, 0);
    hemp_fragments_add_fragment(fragments, space,       text, 0, 0);
    hemp_fragments_add_fragment(fragments, terminator,  text, 0, 0);
    hemp_fragments_add_fragment(fragments, terminator,  text, 0, 0);
    hemp_fragments_add_fragment(fragments, space,       text, 0, 0);
    /* skipping terminators should stop here */
    hemp_fragments_add_fragment(fragments, HempElementText,       text, 0, 0);
    hemp_fragments_add_eof(fragments, 0);

    HempFragment frag = fragments->head;
    
    hemp_skip_delimiter(&frag);
    ok( frag->type == semicolon, "blocked at semicolon separator" );

    hemp_skip_separator(&frag);
    ok( frag->type == terminator, "blocked at terminator" );

    hemp_skip_terminator(&frag);
    ok( frag->type == HempElementText, "blocked at text" );

    hemp_fragments_free(fragments);
    hemp_element_free(space);
    hemp_element_free(comma);
    hemp_element_free(semicolon);
    hemp_element_free(terminator);
    hemp_free(hemp);    
}

