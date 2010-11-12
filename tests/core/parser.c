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
    hemp_str_p text = "DUMMY TEXT";

    hemp_p hemp = hemp_init();
    ok( hemp, "created hemp" );

    hemp_elements_p elements = hemp_elements_init(hemp, 0);
    ok( elements, "created elements" );

    hemp_symbol_p space, comma, semicolon, terminator;
    
    HEMP_TRY;
        space = hemp_symbol(hemp, "hemp.space", " ", NULL);
        ok( space, "got %s symbol", space->name );

        comma = hemp_symbol(hemp, "hemp.delimiter", ",", NULL);
        ok( comma, "got %s symbol", comma->name );

        semicolon = hemp_symbol(hemp, "hemp.separator", ";", NULL);
        ok( semicolon, "got %s symbol", semicolon->name );

        terminator = hemp_symbol(hemp, "hemp.terminator", ".", NULL);
        ok( terminator, "got %s symbol", terminator->name );

    HEMP_CATCH_ALL;
        fail("unexpected error: %s", hemp->error->message);

    HEMP_END;

    hemp_elements_append(elements, space,       text, 0, 0);
    hemp_elements_append(elements, comma,       text, 0, 0);
    hemp_elements_append(elements, space,       text, 0, 0);
    hemp_elements_append(elements, comma,       text, 0, 0);
    /* skipping delimiters should stop here */
    hemp_elements_append(elements, semicolon,   text, 0, 0);
    hemp_elements_append(elements, space,       text, 0, 0);
    hemp_elements_append(elements, semicolon,   text, 0, 0);
    hemp_elements_append(elements, comma,       text, 0, 0);
    hemp_elements_append(elements, space,       text, 0, 0);
    /* skipping separators should stop here */
    hemp_elements_append(elements, terminator,  text, 0, 0);
    hemp_elements_append(elements, space,       text, 0, 0);
    hemp_elements_append(elements, comma,       text, 0, 0);
    hemp_elements_append(elements, space,       text, 0, 0);
    hemp_elements_append(elements, semicolon,   text, 0, 0);
    hemp_elements_append(elements, space,       text, 0, 0);
    hemp_elements_append(elements, terminator,  text, 0, 0);
    hemp_elements_append(elements, terminator,  text, 0, 0);
    hemp_elements_append(elements, space,       text, 0, 0);
    /* skipping terminators should stop here */
    hemp_elements_append(elements, HempSymbolText,       text, 0, 0);
    hemp_elements_eof(elements, 0);

    hemp_element_p elem = elements->head;
    
    hemp_skip_delimiter(&elem);
    ok( elem->type == semicolon, "blocked at semicolon separator" );

    hemp_skip_separator(&elem);
    ok( elem->type == terminator, "blocked at terminator" );

    hemp_skip_terminator(&elem);
    ok( elem->type == HempSymbolText, "blocked at text" );

    hemp_elements_free(elements);
    hemp_symbol_free(space);
    hemp_symbol_free(comma);
    hemp_symbol_free(semicolon);
    hemp_symbol_free(terminator);
    hemp_free(hemp);    
}

