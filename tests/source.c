#include <hemp.h>
#include "tap.h"

void test_source();


int
main(int argc, char **argv, char **env)
{
    plan_tests(8);
    test_source();
    hemp_mem_trace_ok();
    return exit_status();
}


void test_source() {
    hemp_p hemp = hemp_init();
    hemp_scheme_p scheme;
    hemp_source_p source;
    hemp_cstr_p   text;

    ok( hemp, "created hemp object" );

    

    ok(
        (source = hemp_source(hemp, HEMP_TEXT, "source/text.html")),
        "created text source"
    );
    ok( 
        (text = hemp_source_read(source)),
        "read text: %s", text
    );
    ok(
        /* second time around the text should be cached in source->text */
        (text = hemp_source_read(source)),
        "read text again: %s", text
    );

    hemp_source_free(source);

    ok(
        (source = hemp_source(hemp, HEMP_FILE, "source/file.html")),
        "created file source"
    );
    ok( 
        (text = hemp_source_read(source)),
        "read file: %s", text
    );
    ok( 
        (text = hemp_source_read(source)),
        "read file again: %s", text
    );

    hemp_source_free(source);

    hemp_free(hemp);
}