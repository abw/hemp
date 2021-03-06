#include <hemp/test.h>

void test_source();


int main(
    int argc, char **argv, char **env
) {
    plan(7);
    test_source();
    return done();
}


void test_source() {
    Hemp hemp = hemp_new();
    HempSource source;
    HempString text;

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
    
    HempString path  = hemp_filesystem_join_path(HEMP_TEST_DIR, "data/file1");

    ok(
        (source = hemp_source(hemp, HEMP_FILE, path)),
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

    hemp_mem_free(path);

    hemp_source_free(source);

    hemp_free(hemp);
}
