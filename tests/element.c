#include <hemp.h>
#include <tap.h>

void test_element();
void test_element_factory();

int
main(
    int argc, 
    char **argv, 
    char **env
) {
    plan_tests(7);

    test_element();
    hemp_mem_trace_ok();
    hemp_mem_trace_reset();

    test_element_factory();
    hemp_mem_trace_ok();
    hemp_mem_trace_reset();

    return exit_status();
}


void test_element() {
    hemp_element_p element = hemp_element_new();
    ok( element, "created element" );
    hemp_element_free(element);
}


void test_element_factory() {
    hemp_symbol_p symbol;

    hemp_p hemp = hemp_init();
    ok( hemp, "created hemp" );

    HEMP_TRY;
        symbol = hemp_symbol(hemp, "hemp.number.plus", "+", NULL);
        ok( symbol, "got %s symbol", symbol->name );
        hemp_symbol_free(symbol);

        symbol = hemp_symbol(hemp, "hemp.number.plus", "+", NULL);
        ok( symbol, "got it again" );
        hemp_symbol_free(symbol);
    HEMP_CATCH_ALL;
        fail("unexpected error: %s", hemp->error->message);
    HEMP_END;

    HEMP_TRY;
        symbol = hemp_symbol(hemp, "blah.blah", "dud", NULL);
    HEMP_CATCH(HEMP_ERROR_INVALID);
        pass("error as expected: %s", hemp->error->message);
    HEMP_CATCH_ALL;
        fail("unexpected error: %s", hemp->error->message);
    HEMP_END;
    
    hemp_free(hemp);
}
