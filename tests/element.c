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
    hemp_p hemp = hemp_init();
    ok( hemp, "created hemp" );
    
    hemp_element_p elem = hemp_element(hemp, "hemp.number.add");
    ok( elem, "got hemp.number.add element" );
    printf("ELEM: %s\n", (char *) elem);

    elem = hemp_element(hemp, "hemp.number.add");
    ok( elem, "got it again" );
    printf("ELEM: %s\n", (char *) elem);

    HEMP_TRY;
        elem = hemp_element(hemp, "blah.blah");
    HEMP_CATCH(HEMP_ERROR_INVALID);
        pass("caught invalid error");
    HEMP_END;
    
    hemp_free(hemp);
}
