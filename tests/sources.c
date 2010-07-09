#include <stdio.h>
#include "hemp.h"
#include "tap.h"

void test_sources();


int
main(int argc, char **argv, char **env)
{
    plan_tests(6);
    test_sources();
    return exit_status();
}


void test_sources() {
    hemp_scheme_t scheme;
    hemp_source_t source;
    hemp_cstr_t   text;

    ok(
        (source = hemp_source(HEMP_TEXT, "source/text.html")),
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
        (source = hemp_source(HEMP_FILE, "source/file.html")),
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
}
