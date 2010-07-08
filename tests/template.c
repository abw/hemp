#include <stdio.h>
#include "hemp.h"

void test_template();

int
main(int argc, char **argv, char **env)
{
    test_template();
    return 0;
}


void test_template() {
    hemp_template_t tmpl = hemp_template_init(HEMP_TEXT, "hello world!", NULL);
    
    tmpl 
        ? pass("created template")
        : fail("could not create template");
    
    hemp_template_compile(tmpl);

}
