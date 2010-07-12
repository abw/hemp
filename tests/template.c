#include <stdio.h>
#include "hemp.h"
#include "tap.h"


void test_coment_tag();
void test_template();


int
main(int argc, char **argv, char **env)
{
    plan_tests(2);
//    test_template();
    test_comment_tag();
    return exit_status();
}

void test_comment_tag() {
    hemp_template_t tmpl = hemp_template_init(
        HEMP_TEXT, 
        "hello [# comment -#] world!\n", 
        NULL
    );
    
    ok( tmpl , "created template" );
    ok( hemp_template_compile(tmpl), "compiled template" );
}


void test_template() {
    hemp_template_t tmpl = hemp_template_init(
        HEMP_TEXT, 
        "hello [%  %] world! [% 99 or foo %] and [# comment #]\n%% for x in y\nThe End[? control tag ?]Really the end", 
        NULL
    );
    
    ok( tmpl , "created template" );
    ok( hemp_template_compile(tmpl), "compiled template" );
}
