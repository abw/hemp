#include <hemp/test.h>

void test_element();
void test_element_factory();

int main(
    int argc, char **argv, char **env
) {
    plan(5);

    test_element();
    test_element_factory();

    return done();
}


void test_element() {
    hemp_hemp hemp = hemp_new();
    hemp_element element = hemp_element_new(
        HempSymbolSpace, NULL, 0, 0
    );
    ok( element, "created element" );
    hemp_element_free(element);
    hemp_free(hemp);
}


void test_element_factory() {
    hemp_symbol symbol;

    hemp_hemp hemp = hemp_new();
    ok( hemp, "created hemp" );

    HEMP_TRY;
        symbol = hemp_symbol_instance(hemp, "hemp.number.plus", "+", NULL);
        ok( symbol, "got %s symbol", symbol->name );
        hemp_symbol_free(symbol);

        symbol = hemp_symbol_instance(hemp, "hemp.number.plus", "+", NULL);
        ok( symbol, "got it again" );
        hemp_symbol_free(symbol);
    HEMP_CATCH_ALL;
        fail("unexpected error: %s", hemp->error->message);
    HEMP_END;

    HEMP_TRY;
        symbol = hemp_symbol_instance(hemp, "blah.blah", "dud", NULL);
    HEMP_CATCH(HEMP_ERROR_INVALID);
        pass("error as expected: %s", hemp->error->message);
    HEMP_CATCH_ALL;
        fail("unexpected error: %s", hemp->error->message);
    HEMP_END;
    
    hemp_free(hemp);
}
