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
    hemp_string text = "DUMMY TEXT";

    hemp_hemp hemp = hemp_new();
    ok( hemp, "created hemp" );

    hemp_fragments fragments = hemp_fragments_new(hemp, 0);
    ok( fragments, "created fragments" );

    hemp_symbol space, comma, semicolon, terminator;
    
    HEMP_TRY;
        space = hemp_symbol_instance(hemp, "hemp.space", " ", NULL);
        ok( space, "got %s symbol", space->name );

        comma = hemp_symbol_instance(hemp, "hemp.delimiter", ",", NULL);
        ok( comma, "got %s symbol", comma->name );

        semicolon = hemp_symbol_instance(hemp, "hemp.separator", ";", NULL);
        ok( semicolon, "got %s symbol", semicolon->name );

        terminator = hemp_symbol_instance(hemp, "hemp.terminator", ".", NULL);
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

    hemp_element elem = fragments->head;
    
    hemp_skip_delimiter(&elem);
    ok( elem->type == semicolon, "blocked at semicolon separator" );

    hemp_skip_separator(&elem);
    ok( elem->type == terminator, "blocked at terminator" );

    hemp_skip_terminator(&elem);
    ok( elem->type == HempElementText, "blocked at text" );

    hemp_fragments_free(fragments);
    hemp_symbol_free(space);
    hemp_symbol_free(comma);
    hemp_symbol_free(semicolon);
    hemp_symbol_free(terminator);
    hemp_free(hemp);    
}

