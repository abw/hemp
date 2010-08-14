#include <hemp.h>
#include <tap.h>


void test_md5_static();
void test_md5_dynamic();


int
main(
    int  argc, 
    char **argv, 
    char **env
) {
    plan_tests(12);

    test_md5_dynamic();
    hemp_mem_trace_ok();
    hemp_mem_trace_reset();

    test_md5_static();
    hemp_mem_trace_ok();
    hemp_mem_trace_reset();

    return exit_status();
}


void test_md5_dynamic() {
    hemp_md5_p md5 = hemp_md5_init(NULL);
    ok( md5, "created dynamic md5 object: %p", md5 );

    hemp_md5_update(md5, "hello ", 6);
    pass( "updated md5 object" );

    hemp_md5_update(md5, "world", 5);
    pass( "updated md5 object again" );

    hemp_md5_final(md5);
    pass( "finalised md5 object" );

    ok(
        hemp_cstr_eq(
            md5->output,
            "b746ca570487aa55ab3f1dff25d7949a"
        ),
        "MD5 [%s] matches", md5->output
    );
    
    hemp_md5_free(md5);
}


void test_md5_static() {
    hemp_md5_t md5;

    hemp_md5_init(&md5);
    pass( "using static md5 object" );

    hemp_md5_update(&md5, "hello ", 6);
    pass( "updated static md5 object" );

    hemp_md5_update(&md5, "world", 5);
    pass( "updated static md5 object again" );

    hemp_md5_final(&md5);
    pass( "finalised static md5 object" );

    ok(
        hemp_cstr_eq(
            md5.output,
            "b746ca570487aa55ab3f1dff25d7949a"
        ),
        "MD5 [%s] matches", md5.output
    );
}


