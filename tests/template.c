#include <stdio.h>
#include "hemp.h"
#include "tap.h"


void test_template();


int
main(int argc, char **argv, char **env)
{
    plan_tests(2);
    test_template();
    return exit_status();
}


void test_template() {
    hemp_template_t tmpl = hemp_template_init(
        HEMP_TEXT, 
        "hello world! [% foo %] and [# comment #]\n%% for x in y\nThe End", 
        NULL
    );
    
    ok( tmpl , "created template" );
    ok( hemp_template_compile(tmpl), "compiled template" );
}
