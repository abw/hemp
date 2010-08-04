#include <hemp.h>
#include <tap.h>


void test_filesystem();
void test_file1();


int
main(
    int  argc, 
    char **argv, 
    char **env
) {
    plan_tests(7);

    test_filesystem();
//  hemp_mem_trace_ok();
//  hemp_mem_trace_reset();

    test_file1();
//  hemp_mem_trace_ok();
    return exit_status();
}


void test_filesystem() {
    hemp_p hemp = hemp_init();

    hemp_filesystem_p filesystem = hemp_filesystem_init(hemp, "/foo:/bar/:baz/blam:wam:bam/");
    ok( filesystem, "created filesystem with 5 directories in path" );
    ok( filesystem->path->length == 5, "path has four directories" );
    
    ok( 1, "finding readable paths for wizbang.html" );
    hemp_filesystem_readable_path(filesystem, "wizbang.html");

    ok( 1, "finding readable paths for /wibble.html" );
    hemp_filesystem_readable_path(filesystem, "/wibble.html");

    hemp_filesystem_set_path(filesystem, TESTDIR);
    ok( filesystem, "created filesystem with a single path" );
    ok( filesystem->path->length == 1, "path has one directory" );

    hemp_filesystem_free(filesystem);
    hemp_free(hemp);
}


void test_file1() {
    hemp_p hemp = hemp_init();
    hemp_filesystem_p filesystem = hemp_filesystem_init(hemp, TESTDIR);
    hemp_cstr_p text = hemp_filesystem_read_file(
        hemp_filesystem_join_path(TESTDIR, "data/file1")
    );
    ok( text, "read text from foobar: %s", text );
}
