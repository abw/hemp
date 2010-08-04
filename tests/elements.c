#include <hemp.h>
#include <tap.h>

void test_elements();

int
main(int argc, char **argv, char **env)
{
    plan_tests(4);
    test_elements();
    hemp_mem_trace_ok();
    return exit_status();
}

void test_elements() {
    hemp_elements_p elements = hemp_elements_init(0);
    ok( elements, "created elements" );
    
    hemp_element_p eof = hemp_elements_eof(elements, 0);
    ok( eof, "got EOF element" );
    ok( hemp_has_flag(eof, HEMP_IS_EOF), "HEMP_IS_EOF is set" );
    
    hemp_elements_free(elements);
}
