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
    hemp_hemp hemp = hemp_new();
    hemp_source source;
    hemp_string text;

    ok( hemp, "created hemp object" );

    ok(
        (source = hemp_source_instance(hemp, HEMP_TEXT, "source/text.html")),
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
    
    hemp_string path  = hemp_filesystem_join_path(HEMP_TESTDIR, "data/file1");

    ok(
        (source = hemp_source_instance(hemp, HEMP_FILE, path)),
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
