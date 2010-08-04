#include <hemp.h>
#include <tap.h>

void test_grammar();


int
main(
    int  argc, 
    char **argv, 
    char **env
) {
    plan_tests(1);
    test_grammar();
    hemp_mem_trace_ok();

    return exit_status();
}

void test_grammar() {
    hemp_p hemp = hemp_init();
    ok( hemp, "created hemp object" );

    hemp_free(hemp);
}
