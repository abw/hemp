#include <hemp/test.h>

#define HEMP_FS_DIR  HEMP_TEST_DIR "/data/filesystem"


void test_hemp_filesystem();
void test_filesystem();
void test_file1();


int main(
    int argc, char **argv, char **env
) {
    plan(7);

    test_hemp_filesystem();
    test_filesystem();
    test_file1();
    return done();
}


void test_hemp_filesystem() {
    hemp_hemp hemp = hemp_new();
    ok( hemp, "created hemp" );

    hemp_language_instance(hemp, "json");

    HEMP_TRY;
        hemp_configure_from(
            hemp, "json", "text", 
            "{ hemp: { path: 'here' } }"
        );
        pass("configured from text");
        hemp_ready(hemp);
    
    HEMP_CATCH_ALL;
        fail("caught error: %s", hemp->error->message);
        hemp_text error = hemp_error_text(hemp->error);
        printf("%s", error->string);
        hemp_text_free(error);

    HEMP_END;

    hemp_free(hemp);
}


void test_filesystem() {
    hemp_hemp hemp = hemp_new();

    printf("TESTDIR: %s\n", HEMP_TEST_DIR);
    hemp_string fsdir = HEMP_FS_DIR;
    hemp_string a     = HEMP_FS_DIR "/alpha";
    hemp_string bc    = HEMP_FS_DIR "/bravo/charlie";
    hemp_string def   = HEMP_FS_DIR "/delta/echo/foxtrot/";

    printf("fsdir...%s\na.......%s\nbc......%s\ndef.....%s\n", fsdir, a, bc, def);

    hemp_filesystem filesystem = hemp_filesystem_new(hemp);
    ok( filesystem, "created filesystem" );
    hemp_filesystem_add_root(filesystem, a);
    hemp_filesystem_add_root(filesystem, bc);
    hemp_filesystem_add_root(filesystem, def);
    ok( filesystem->roots->length == 3, "added three roots to filesystem" );
    
    ok( 1, "finding readable paths for wizbang.html" );
    hemp_filesystem_readable_path(filesystem, "wizbang.html");

    ok( 1, "finding readable paths for /wibble.html" );
    hemp_filesystem_readable_path(filesystem, "/wibble.html");

//    hemp_filesystem_set_path(filesystem, HEMP_TEST_DIR);
//    ok( filesystem, "created filesystem with a single path" );
//    ok( filesystem->roots->length == 1, "path has one directory" );

    hemp_filesystem_free(filesystem);
    hemp_free(hemp);
}


void test_file1() {
    hemp_hemp       hemp        = hemp_new();
    hemp_filesystem filesystem  = hemp_filesystem_new(hemp);
    hemp_string path  = hemp_filesystem_join_path(HEMP_TEST_DIR, "data/file1");
    hemp_string text  = hemp_filesystem_read_file(path);
    ok( text, "read text from foobar: %s", text );
    hemp_mem_free(text);
    hemp_mem_free(path);
    hemp_filesystem_free(filesystem);
    hemp_free(hemp);
}
