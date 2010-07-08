#include <stdio.h>
#include "hemp.h"

void test_element();

int
main(int argc, char **argv, char **env)
{
    test_element();
    return 0;
}

void test_element() {
    debug("test_element()\n");
    hemp_element_t element = hemp_element_init();
    element 
        ? pass("created element at %p", element)
        : fail("could not created element");
    hemp_element_free(element);
    pass("freed element");
}
