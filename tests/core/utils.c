#include <hemp/test.h>


void test_md5_static();
void test_md5_dynamic();


int main(
    int argc, char **argv, char **env
) {
    plan(10);
    test_md5_dynamic();
    test_md5_static();
    return done();
}


void test_md5_dynamic() {
    hemp_md5 md5 = hemp_md5_init(NULL);
    ok( md5, "created dynamic md5 object: %p", md5 );

    hemp_md5_update(md5, "hello ", 6);
    pass( "updated md5 object" );

    hemp_md5_update(md5, "world\n", 6);
    pass( "updated md5 object again" );

    hemp_md5_final(md5);
    pass( "finalised md5 object" );

    ok(
        hemp_string_eq(
            (hemp_string) md5->output,
            "6f5902ac237024bdd0c176cb93063dc4"
        ),
        "MD5 [%s] matches", md5->output
    );
    
    hemp_md5_free(md5);
}


void test_md5_static() {
    struct hemp_md5 md5;

    hemp_md5_init(&md5);
    pass( "using static md5 object" );

    hemp_md5_update(&md5, "hello ", 6);
    pass( "updated static md5 object" );

    hemp_md5_update(&md5, "world\n", 6);
    pass( "updated static md5 object again" );

    hemp_md5_final(&md5);
    pass( "finalised static md5 object" );

    ok(
        hemp_string_eq(
            (hemp_string) md5.output,
            (hemp_string) "6f5902ac237024bdd0c176cb93063dc4"
        ),
        "MD5 [%s] matches", md5.output
    );
}


