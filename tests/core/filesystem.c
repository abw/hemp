#include <hemp/test.h>


void test_filesystem();
void test_file1();


int main(
    int argc, char **argv, char **env
) {
    plan(7);

    test_filesystem();
    test_file1();

    return done();
}


void test_filesystem() {
    hemp_hemp hemp = hemp_init();

    hemp_filesystem_p filesystem = hemp_filesystem_init(hemp, "/foo:/bar/:baz/blam:wam:bam/");
    ok( filesystem, "created filesystem with 5 directories in path" );
    ok( filesystem->path->length == 5, "path has four directories" );
    
    ok( 1, "finding readable paths for wizbang.html" );
    hemp_filesystem_readable_path(filesystem, "wizbang.html");

    ok( 1, "finding readable paths for /wibble.html" );
    hemp_filesystem_readable_path(filesystem, "/wibble.html");

    hemp_filesystem_set_path(filesystem, HEMP_TESTDIR);
    ok( filesystem, "created filesystem with a single path" );
    ok( filesystem->path->length == 1, "path has one directory" );

    hemp_filesystem_free(filesystem);
    hemp_free(hemp);
}


void test_file1() {
    hemp_hemp hemp      = hemp_init();
    hemp_filesystem_p filesystem 
                     = hemp_filesystem_init(hemp, HEMP_TESTDIR);
    hemp_string path  = hemp_filesystem_join_path(HEMP_TESTDIR, "data/file1");
    hemp_string text  = hemp_filesystem_read_file(path);
    ok( text, "read text from foobar: %s", text );
    hemp_mem_free(text);
    hemp_mem_free(path);
    hemp_filesystem_free(filesystem);
    hemp_free(hemp);
}
