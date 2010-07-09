#include <stdio.h>
#include "hemp.h"
#include "tap.h"

void test_element();

int
main(int argc, char **argv, char **env)
{
    plan_tests(2);
    test_element();
    return exit_status();
}

void test_element() {
    hemp_element_t element = hemp_element_init();
    ok( element, "created element" );
    hemp_element_free(element);
    pass("released element");
}
