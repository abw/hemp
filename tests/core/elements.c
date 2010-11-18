#include <hemp/test.h>


void test_elements();

int main(
    int argc, char **argv, char **env
) {
    plan(4);
    test_elements();
    return done();
}

void test_elements() {
    hemp_p hemp = hemp_init();
    ok( hemp, "created hemp" );

    hemp_elements_p elements = hemp_elements_init(hemp, 0);
    ok( elements, "created elements" );
    
    hemp_element_p eof = hemp_elements_eof(elements, 0);
    ok( eof, "got EOF element" );
    ok( hemp_has_flag(eof, HEMP_BE_EOF), "HEMP_BE_EOF is set" );

    hemp_elements_free(elements);
    hemp_free(hemp);
}
